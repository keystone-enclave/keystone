#include "call/callee.h"

#ifdef USE_CALLEE

// These defines are necessary so that we can reach
// the definitions of CLONE_* used by libc

#define __USE_GNU
#define __USE_XOPEN2K
#include <sched.h>
#undef __USE_XOPEN2K
#undef __USE_GNU

#include <asm-generic/errno.h>
#include <stdbool.h>

#include "asm/csr.h"
#include "mm/common.h"
#include "mm/mm.h"
#include "call/sbi.h"
#include "uaccess.h"
#include "mm/vm.h"
#include "util/spinlock.h"

/* Low-level thread availability tracking */

typedef enum {
  STATE_EMPTY = 0,
  STATE_READY,
  STATE_RUNNING
} thread_state_t;

extern uintptr_t callee_kstacks_start;
extern uintptr_t callee_kstacks_end;

static struct {
  thread_state_t state;
  uintptr_t sp;
  uintptr_t ksp;
  uintptr_t gp;
  uintptr_t tp;
  uintptr_t sepc;
  uintptr_t clear_child_tid;
} threadinfo[NUM_THREAD_SLOTS];

spin_lock_t threadinfo_lock = 0;

/* Interface to the eapp and SDK */

static bool encl_call_registered = false;
static uintptr_t encl_call_buf;
static int encl_call_buflen;

// Used externally in callee.S
uintptr_t encl_call_lock;

/* Call handler, used by callees into this enclave */
struct encl_call_regs {
  uint64_t a0;
  uint64_t a1;
  uint64_t a2;
  uint64_t a3;
  uint64_t a4;
  uint64_t a5;
  uint64_t a6;
  uint64_t a7;

  uint64_t tp;
  uint64_t gp;
  uint64_t ksp;
};

uintptr_t encl_call_handler_c(struct encl_call_regs *regs)  {
  int i;
  uintptr_t ret = 0;

  spin_lock(&threadinfo_lock);
  for(i = 0; i < NUM_THREAD_SLOTS; i++) {
    if(threadinfo[i].state == STATE_READY) {
      // We can run this thread!
      threadinfo[i].state = STATE_RUNNING;

      // Copy registers into the user buffer, which we have
      // exclusively acquired for this thread only
      assert(encl_call_buflen <= 5 * sizeof(uint64_t));
      copy_to_user((void *) encl_call_buf, regs, encl_call_buflen);

      // Write necessary info
      csr_write(sepc, threadinfo[i].sepc);
      regs->tp = threadinfo[i].tp;
      regs->gp = threadinfo[i].gp;
      regs->ksp = threadinfo[i].ksp;

      // Return stack pointer to pop
      ret = threadinfo[i].sp;
      break;
    }
  }

  spin_unlock(&threadinfo_lock);
  return ret;
}

/* Setup infrastructure, used by the main thread in this enclave */

extern void* encl_call_handler;

int initialize_call_handler(uintptr_t argbuf, int arglen, uintptr_t arglock) {
  int ret = -1;

  // Check arguments
  if(!argbuf || !arglock || arglen > 5 * sizeof(uint64_t)) {
    return -1;
  }

  if(!encl_call_registered) {
    ret = sbi_register_handler((uintptr_t) &encl_call_handler);
    if(ret < 0)
      return ret;

    encl_call_buf = argbuf;
    encl_call_buflen = arglen;
    encl_call_lock = arglock;
    encl_call_registered = true;
  }

  return ret;
}


int syscall_clone(unsigned long flags, uintptr_t newsp,
              int *parent_tidptr, uintptr_t tls, int *child_tidptr,
              uintptr_t gp /* needed for switching */ ) {
  int i;

  spin_lock(&threadinfo_lock);
  for(i = 0; i < NUM_THREAD_SLOTS; i++) {
    if(threadinfo[i].state == STATE_EMPTY) {
      // Allocate a (small) kernel stack for this thread, if one
      // has not been allocated yet. Fail if this is not possible.
      if(!threadinfo[i].ksp)
        threadinfo[i].ksp = (uint64_t) &callee_kstacks_end - 0x1000 * i;

      threadinfo[i].state = STATE_READY;
      threadinfo[i].sp = newsp;
      threadinfo[i].gp = gp;
      threadinfo[i].sepc = csr_read(sepc) + 4;

      if(flags & CLONE_SETTLS)
        threadinfo[i].tp = tls;

      if(flags & CLONE_CHILD_CLEARTID)
        threadinfo[i].clear_child_tid = (uintptr_t) child_tidptr;

      break;
    }
  }
  spin_unlock(&threadinfo_lock);

  if(i == NUM_THREAD_SLOTS){
    // no free thread slots
    return -ENOMEM;
  } else {
    // Propagate the thread id where it needs to go
    i++;
    if(flags & CLONE_PARENT_SETTID) {
      copy_to_user(parent_tidptr, &i, sizeof(int));
    }

    if(flags & CLONE_CHILD_SETTID) {
      copy_to_user(child_tidptr, &i, sizeof(int));
    }

    return i;
  }
}

int syscall_set_tid_address(void *stack, uintptr_t tp, int *tidptr_t) {
  int i, ret = -EINVAL;

  spin_lock(&threadinfo_lock);
  for(i = 0; i < NUM_THREAD_SLOTS; i++) {
    if(threadinfo[i].state == STATE_RUNNING &&
        threadinfo[i].tp == tp &&
        threadinfo[i].ksp == (uintptr_t) stack) {
      // This is the desired entry
      threadinfo[i].clear_child_tid = (uintptr_t) tidptr_t;
      ret = 0;
    }
  }

  spin_unlock(&threadinfo_lock);
  return ret;
}

int free_thread_entry(uintptr_t tp, uintptr_t ksp) {
  int i, ret = -1;

  spin_lock(&threadinfo_lock);
  for(i = 0; i < NUM_THREAD_SLOTS; i++) {
    if(threadinfo[i].state == STATE_RUNNING &&
        threadinfo[i].tp == tp &&
        threadinfo[i].ksp == ksp) {

      threadinfo[i].state = STATE_EMPTY;
      threadinfo[i].gp = 0;
      threadinfo[i].tp = 0;
      threadinfo[i].sepc = 0;

      // We don't free the stack here, since libc will actually
      // transparently reuse it for us in new threads. If libc
      // decides the stack needs to go, it'll call munmap
      threadinfo[i].sp = 0;
      ret = 0;

      if(threadinfo[i].clear_child_tid) {
        // Use ret as a source value since we conveniently just set it to 0
        copy_to_user((void *) threadinfo[i].clear_child_tid, &ret, sizeof(int));
        threadinfo[i].clear_child_tid = 0;
      }

      break;
    }
  }

  spin_unlock(&threadinfo_lock);
  return ret;
}

/* Miscellaneous helpers */

bool is_callee(void *stack) {
  void *ptr;

  for(ptr = ((void *) &callee_kstacks_start) + RISCV_PAGE_SIZE ;
       ptr <= (void *) &callee_kstacks_end; ptr += RISCV_PAGE_SIZE) {
    if(ptr == stack)
      return true;
  }

  return false;
}

#endif // USE_CALLEE
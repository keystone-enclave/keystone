#include "callee.h"

#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include "syscall.h"

/*******************************/
/** Callee-side functionality **/
/*******************************/

#define MAX_CALLEE_THREADS  8
static struct tinfo {
  int tid;
  call_type_t type;
  pthread_t handle;
  int (*fn)(void *);
} callee_threadinfo[MAX_CALLEE_THREADS];

static pthread_attr_t callee_attrs;


#define MAX_ARGUMENTS   5
static uint64_t argbuf[MAX_ARGUMENTS];
static pthread_spinlock_t arglock;

void *dispatch_callee_handler(void *arg) {
  int fn_ret;
  uint64_t call_args[MAX_ARGUMENTS];
  call_type_t call_type;

  struct tinfo *tinfo = arg;
  int (*fn)(void *) = tinfo->fn;

  // Pull arguments from stack
  memcpy(call_args, argbuf, sizeof(argbuf));
  memset(argbuf, 0, sizeof(argbuf));

  // This is the earliest point in time at which we can
  // release the above spinlock, enabling further threads to spawn
  pthread_spin_unlock(&arglock);

  // Check that the type of call which was received matches what
  // we expected for this thread. If not, return an error code
  call_type = call_args[0];
  if(call_type != tinfo->type) {
    fn_ret = -EINVAL;
    goto __done;
  }

  switch(tinfo->type) {
    // Dispatch these arguments directly to the actual function
    case CALL_RECEIVER:
      fn_ret = fn(&call_args[1]);
      break;

    default:
      fn_ret = -EINVAL;
      break;
  }

  // The handler has finished! Create another thread to handle
  // another call, so we keep propagating this slot. Note that if
  // this call fails, future callee handlers in the runtime will
  // start to die as they are not replaced. This will lead to errors
  // in the sender, and leaving warnings about this in the receiver
  // (here) is left as a TODO

__done:
  pthread_create(&tinfo->handle, &callee_attrs,
                       dispatch_callee_handler, tinfo);

  return (void *) (uintptr_t) fn_ret;
}

int spawn_callee_handler(int (*fn) (void *), call_type_t type) {
  int i, ret = -1;

  // For functions called in the receiver, we need to ensure that
  // a function to dispatch to actually exists.

  if((type == CALL_RECEIVER && !fn)) {
    return -1;
  }

  for(i = 0; i < MAX_CALLEE_THREADS; i++) {
    if(callee_threadinfo[i].tid == 0) {
      callee_threadinfo[i].tid = i + 1;
      callee_threadinfo[i].type = type;
      callee_threadinfo[i].fn = fn;

      return pthread_create(&callee_threadinfo[i].handle,
                            &callee_attrs,
                            dispatch_callee_handler,
                            &callee_threadinfo[i]);
    }
  }

  return ret;
}

int init_callee(void) {
  int ret;

  // Initialize global state
  pthread_spin_init(&arglock, PTHREAD_PROCESS_PRIVATE);
  memset(argbuf, 0, sizeof(argbuf));

  // Need to limit stack size since the default is kinda
  // huge for an embedded platform such as keystone.
  pthread_attr_setstacksize(&callee_attrs, 0x8000);

  // Create callee threads detached, since we won't explicitly join
  // with them later and thread resources (notably stacks) must be reclaimed
  pthread_attr_setdetachstate(&callee_attrs, PTHREAD_CREATE_DETACHED);

  // Register argument buffer with the runtime/SM
  ret = register_handler((uintptr_t) argbuf, sizeof(argbuf),
                         (uintptr_t) &arglock);
  if(ret < 0) {
    return ret;
  }

  return 0;
}

/*******************************/
/** Caller-side functionality **/
/*******************************/

uint64_t call_enclave(int eid, call_type_t type, int nargs, ...) {
  int i;
  va_list ptr;
  uint64_t args[4] = {0};

  // Cannot currently pass more than 5 registers
  if(nargs > 4) {
    return -1;
  }

  va_start(ptr, nargs);
  for(i = 0; i < nargs; i++) {
    args[i] = va_arg(ptr, uint64_t);
  }
  va_end(ptr);

  return SYSCALL_6(RUNTIME_SYSCALL_CALL_ENCLAVE, eid, type,
                   args[0], args[1], args[2], args[3]);
}

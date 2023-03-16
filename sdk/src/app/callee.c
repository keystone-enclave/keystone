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
typedef int (called_function_t)(void *);
static struct tinfo {
  int tid;
  call_type_t type;
  pthread_t handle;
  called_function_t*fn;
} callee_threadinfo[MAX_CALLEE_THREADS];

static pthread_attr_t callee_attrs;

#define MAX_ARGUMENTS   5
static uint64_t argbuf[MAX_ARGUMENTS], asyncbuf[MAX_ARGUMENTS];
static pthread_spinlock_t arglock, asynclock;
bool async_running, async_ready;

called_function_t* map_exported_function(uint64_t args[MAX_ARGUMENTS]) {
  int (*fn)(void *) = (void *) map(args[1], args[2], args[3]);
  if(!fn) {
    return NULL;
  }

  if((uintptr_t) fn != args[3]) {
    // Function was mapped to the wrong address
    unmap((uintptr_t) fn, args[2]);
    return NULL;
  }

  return fn;
}

__attribute__((noreturn)) void catch_async_threads() {
  pthread_spin_init(&asynclock, PTHREAD_PROCESS_PRIVATE);

  uint64_t call_args[MAX_ARGUMENTS];
  called_function_t *fn;
  async_running = false;
  async_ready = false;

  while(1) {
    pthread_spin_lock(&asynclock);

    if(async_ready) {
      memcpy(call_args, asyncbuf, sizeof(asyncbuf));
      memset(asyncbuf, 0, sizeof(asyncbuf));

      fn = map_exported_function(call_args);
      if(fn) {
        async_ready = false;
        async_running = true;
        pthread_spin_unlock(&asynclock);

        fn((void *) call_args[4]);
        unmap(call_args[3], call_args[2]);
      }
    } else {
      pthread_spin_unlock(&asynclock);
    }
  }
}

void *dispatch_callee_handler(void *arg) {
  int fn_ret;
  uint64_t call_args[MAX_ARGUMENTS];
  call_type_t call_type;

  struct tinfo *tinfo = arg;
  called_function_t*fn = tinfo->fn;

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

    // Need to:
    // 1. Map requested virtual memory from caller
    // 2. Indicate existing eapp space somehow? Library functionality
    // 3. Call into the specified function
    case CALL_MAPPED:
      fn = map_exported_function(call_args);
      if(fn) {
        fn_ret = fn((void*) call_args[4]);
        unmap(call_args[3], call_args[2]);
      } else {
        fn_ret = -ENOMEM;
      }
      break;

    case CALL_MAPPED_ASYNC:
      pthread_spin_lock(&asynclock);

      if(!async_ready) {
        memcpy(asyncbuf, call_args, sizeof(asyncbuf));
        async_ready = true;
        fn_ret = 0;
      } else {
        fn_ret = -EBUSY;
      }

      pthread_spin_unlock(&asynclock);
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

int spawn_callee_handler(called_function_t*fn, call_type_t type) {
  int i, ret = -1;

  // For functions called in the receiver, we need to ensure that
  // a function to dispatch to actually exists. Otherwise, the
  // function's virtual address is passed to us at call time and
  // we will manually map it then.

  if((type == CALL_RECEIVER && !fn) ||
      ((type == CALL_MAPPED || type == CALL_MAPPED_ASYNC) && fn)) {
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

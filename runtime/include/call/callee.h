#ifndef _CALLEE_H_
#define _CALLEE_H_

#ifdef USE_CALLEE

#define NUM_THREAD_SLOTS 8

#ifndef __PREPROCESSING__
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

bool is_callee(void *stack);

int initialize_call_handler(uintptr_t argbuf, int arglen, uintptr_t arglock);
int free_thread_entry(uintptr_t tp, uintptr_t ksp);

int syscall_clone(unsigned long flags, uintptr_t newsp,
              int *parent_tidptr, uintptr_t tls, int *child_tidptr,
              uintptr_t gp /* needed for switching */);
int syscall_set_tid_address(void *stack, uintptr_t tp, int *tidptr_t);
#endif // __PREPROCESSING__

#endif // USE_CALLEE

#endif  // _CALLEE_H_

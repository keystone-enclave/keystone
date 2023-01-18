

#ifndef __CALLEE_H__
#define __CALLEE_H__

#include <pthread.h>
#include <stdint.h>

typedef enum {
  CALL_RECEIVER = 0,
} call_type_t;

int init_callee(void);

int spawn_callee_handler(int (*fn) (void *), call_type_t type);

uint64_t call_enclave(int eid, call_type_t type, int nargs, ...);

#endif  // __CALLEE_H__

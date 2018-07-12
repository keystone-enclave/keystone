#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#include "pmp.h"

typedef enum {
  INVALID = -1,
  FRESH = 0,
  INITIALIZED,
  RUNNING,
} enclave_state_t;

struct enclave_t
{
  int eid; //enclave id
  int rid; //region id
  enclave_state_t state;
};

int create_enclave(uintptr_t base, uintptr_t size);
int destroy_enclave(int eid);

#endif

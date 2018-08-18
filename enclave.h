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
  unsigned long host_satp;
  unsigned long encl_satp;
  enclave_state_t state;
  uintptr_t mepc;
};

unsigned long get_host_satp(int eid);
int create_enclave(uintptr_t base, uintptr_t size);
int destroy_enclave(int eid);
int copy_to_enclave(int eid, uintptr_t addr, uintptr_t ptr, size_t size);
int copy_from_enclave(int eid, void* ptr, size_t size);
int run_enclave(int eid, uintptr_t ptr);

#endif

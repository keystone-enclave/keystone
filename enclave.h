#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#include "pmp.h"
#include "page.h"

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
  pgd_t* ptbr; //page table base
  unsigned long host_satp;
  enclave_state_t state;
};

unsigned long get_host_satp(int eid);
int create_enclave(uintptr_t base, uintptr_t size);
int destroy_enclave(int eid);
int copy_to_enclave(int eid, void* ptr, size_t size);
int copy_from_enclave(int eid, void* ptr, size_t size);
#endif

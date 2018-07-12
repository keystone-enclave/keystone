#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"

int mcall_sm_create_enclave(unsigned long base, unsigned long size)
{
  return create_enclave((uintptr_t) base, (uintptr_t) size);
}

int mcall_sm_destroy_enclave(unsigned long eid)
{
  /* Implement Me */
  return 0;
}

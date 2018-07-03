#include "sm-sbi.h"

uintptr_t mcall_sm_create_enclave()
{
  return 0xDEADBEEF;
}

uintptr_t mcall_sm_destroy_enclave()
{
  return 0xDEADBEEF;
}

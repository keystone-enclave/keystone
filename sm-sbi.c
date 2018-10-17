#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"
#include <errno.h>

uintptr_t mcall_sm_create_enclave(unsigned long base, unsigned long size)
{
  int ret;
  ret = create_enclave((uintptr_t) base, (uintptr_t) size);
  return ret;
}

uintptr_t mcall_sm_destroy_enclave(unsigned long eid)
{
  uintptr_t ret;
  if(get_host_satp(eid) != read_csr(satp))
    return ENCLAVE_NOT_ACCESSIBLE;
  ret = destroy_enclave((int)eid);
  return ret;
}

uintptr_t mcall_sm_run_enclave(unsigned long eid, unsigned long ptr, unsigned long retval)
{
  if(get_host_satp(eid) != read_csr(satp))
    return ENCLAVE_NOT_ACCESSIBLE;
  return run_enclave((unsigned int) eid, (uintptr_t) ptr, (uintptr_t) retval);
}

uintptr_t mcall_sm_exit_enclave(unsigned long retval)
{
  return exit_enclave((unsigned long) retval);
}

uintptr_t mcall_sm_not_implemented(unsigned long cause)
{
  if((long)cause < 0)
  {
    // discard MSB
    cause = cause << 1;
    cause = cause >> 1;
    printm("the runtime could not handle interrupt %ld\n", cause );
  }
  else
  {
    printm("the runtime could not handle exception %ld\n", cause);
  }
  return exit_enclave((uint64_t)-1UL);
}

#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"
#include <errno.h>

uintptr_t mcall_sm_create_enclave(unsigned long base, unsigned long size, unsigned long eidptr)
{
  enclave_ret_t ret;
  ret = create_enclave((uintptr_t) base, (size_t) size, (unsigned int*) eidptr);
  return ret;
}

uintptr_t mcall_sm_destroy_enclave(unsigned long eid)
{
  enclave_ret_t ret;
  if(get_host_satp(eid) != read_csr(satp))
    return ENCLAVE_NOT_ACCESSIBLE;
  ret = destroy_enclave((unsigned int)eid);
  return ret;
}

uintptr_t mcall_sm_run_enclave(uintptr_t* host_regs, unsigned long eid, unsigned long ptr, unsigned long retval)
{
  if(get_host_satp(eid) != read_csr(satp))
    return ENCLAVE_NOT_ACCESSIBLE;
  return run_enclave(host_regs, (unsigned int) eid, (uintptr_t) ptr, (unsigned long*) retval);
}

uintptr_t mcall_sm_resume_enclave(uintptr_t* host_regs, unsigned long eid)
{
  if(get_host_satp(eid) != read_csr(satp))
    return ENCLAVE_NOT_ACCESSIBLE;
  return resume_enclave(host_regs, (unsigned int) eid);
}

uintptr_t mcall_sm_exit_enclave(uintptr_t* encl_regs, unsigned long retval)
{
  return exit_enclave(encl_regs, (unsigned long) retval);
}

uintptr_t mcall_sm_stop_enclave(uintptr_t* encl_regs, unsigned long request)
{
  return stop_enclave(encl_regs, (uint64_t)request);
}

uintptr_t mcall_sm_not_implemented(uintptr_t* encl_regs, unsigned long cause)
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
  return exit_enclave(encl_regs, (uint64_t)-1UL);
}

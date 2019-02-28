//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"
#include <errno.h>
#include "page.h"

uintptr_t mcall_sm_create_enclave(uintptr_t create_args)
{
  struct keystone_sbi_create_t create_args_local;
  enclave_ret_t ret;

  ret = copy_from_host((struct keystone_sbi_create_t*)create_args,
                       &create_args_local,
                       sizeof(struct keystone_sbi_create_t));

  if( ret != ENCLAVE_SUCCESS )
    return ret;

  ret = create_enclave(create_args_local);
  return ret;
}

uintptr_t mcall_sm_destroy_enclave(unsigned long eid)
{
  enclave_ret_t ret;
  unsigned long host_satp;
  if(get_host_satp(eid, &host_satp) != ENCLAVE_SUCCESS ||
     host_satp != read_csr(satp))
    return ENCLAVE_NOT_ACCESSIBLE;
  ret = destroy_enclave((unsigned int)eid);
  return ret;
}
uintptr_t mcall_sm_run_enclave(uintptr_t* regs, unsigned long eid)
{
  enclave_ret_t ret;
  unsigned long host_satp;
  if(get_host_satp(eid, &host_satp) != ENCLAVE_SUCCESS ||
     host_satp != read_csr(satp))
    return ENCLAVE_NOT_ACCESSIBLE;

  ret = run_enclave(regs, (unsigned int) eid);

  return ret;
}

uintptr_t mcall_sm_resume_enclave(uintptr_t* host_regs, unsigned long eid)
{
  unsigned long host_satp;
  if(get_host_satp(eid, &host_satp) != ENCLAVE_SUCCESS ||
     host_satp != read_csr(satp))
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

uintptr_t mcall_sm_attest_enclave(uintptr_t report, uintptr_t data, uintptr_t size)
{
  return attest_enclave(report, data, size);
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

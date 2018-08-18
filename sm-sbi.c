#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"
#include <errno.h>

int mcall_sm_create_enclave(unsigned long base, unsigned long size)
{
  return create_enclave((uintptr_t) base, (uintptr_t) size);
}

int mcall_sm_destroy_enclave(unsigned long eid)
{
  if(get_host_satp(eid) != read_csr(satp))
    return -EFAULT;
  return destroy_enclave((int)eid);
}

int mcall_sm_copy_from_enclave(unsigned long eid, unsigned long ptr, unsigned long size)
{
  if(get_host_satp(eid) != read_csr(satp))
    return -EFAULT;
  return copy_from_enclave(eid, (void*) ptr, (size_t) size);
}

int mcall_sm_copy_to_enclave(unsigned long eid, unsigned long addr, unsigned long ptr, unsigned long size)
{
  if(get_host_satp(eid) != read_csr(satp))
    return -EFAULT;
  return copy_to_enclave(eid, (uintptr_t) addr,(uintptr_t) ptr, (size_t) size);
}

int mcall_sm_run_enclave(unsigned long eid, unsigned long ptr)
{
  if(get_host_satp(eid) != read_csr(satp))
    return -EPERM;
  return run_enclave((unsigned int) eid, (uintptr_t) ptr);
}

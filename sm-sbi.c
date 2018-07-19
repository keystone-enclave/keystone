#include "sm-sbi.h"
#include "pmp.h"
#include "enclave.h"

int mcall_sm_create_enclave(unsigned long base, unsigned long size)
{
  unsigned long ptbr = read_csr(satp);
  printm("ptbr: 0x%llx\n", ptbr);
  return create_enclave((uintptr_t) base, (uintptr_t) size);
}

int mcall_sm_destroy_enclave(unsigned long eid)
{
  unsigned long ptbr = read_csr(satp);
  printm("ptbr: 0x%llx\n", ptbr);
  return destroy_enclave((int)eid);
}

int mcall_sm_copy_from_enclave(unsigned long ptr, unsigned long size)
{
  unsigned long ptbr = read_csr(satp);
  printm("ptbr: 0x%llx\n", ptbr);
  return copy_from_enclave(0, (void*) ptr, (size_t) size);
}

int mcall_sm_copy_to_enclave(unsigned long ptr, unsigned long size)
{
  unsigned long ptbr = read_csr(satp);
  printm("ptbr: 0x%llx\n", ptbr);
  return copy_to_enclave(0, (void*) ptr, (size_t) size);
}

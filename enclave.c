#include "bits.h"
#include "vm.h"
#include "enclave.h"
#include "pmp.h"
#include <string.h>

#define ENCL_MAX  16
static uint64_t encl_bitmap = 0;
static int running_encl_id = -1;

extern void save_host_regs(void);

#define RET_ON_ERR(ret) {if(ret<0) return ret;}

struct enclave_t enclaves[ENCL_MAX];

int encl_alloc_idx()
{
  int i;
  for(i=0; i<ENCL_MAX; i++)
  {
    if(!(encl_bitmap & (0x1 << i)))
      break;
  }

  if(i == ENCL_MAX)
    return -1;

  SET_BIT(encl_bitmap, i); 
  return i;
}

int encl_free_idx(int idx)
{
  if(!TEST_BIT(encl_bitmap, idx))
    return -1;

  UNSET_BIT(encl_bitmap, idx);
  return 0;
}

unsigned long get_host_satp(int eid)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;

  return enclaves[eid].host_satp;
}

int create_enclave(uintptr_t base, uintptr_t size)
{
  uint8_t perm = 0;
  int ret, region;
  
  // TODO: check if base/size legitimate
  // - if size larger than minimum requirement (16 KB)
  // - if base and (base+size) not belong to other enclaves
  // - if size is multiple of 4KB (smaller than 4KB not supported) 

  // 1. reserve pmp range
  ret = pmp_region_init(base, size, perm);
  RET_ON_ERR(ret);

  region = ret;

  // 2. set pmp
  ret = pmp_set(region); 
  RET_ON_ERR(ret);

  // 3. allocate eid
  ret = encl_alloc_idx();
  RET_ON_ERR(ret);

  enclaves[ret].eid = ret;
  enclaves[ret].rid = region;
  enclaves[ret].state = FRESH;
  enclaves[ret].host_satp = read_csr(satp);
  enclaves[ret].encl_satp = ((base >> RISCV_PGSHIFT) | SATP_MODE_CHOICE);
  return 0;
}

int destroy_enclave(int eid)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;

  // 1. clear all the data in the enclave page
  // TODO
  
  // 2. free pmp range pages
  // TODO Page table not implemented 
  
  // 3. unset pmp
  pmp_region_free(enclaves[eid].rid);
  // 4. release eid
  encl_free_idx(eid);
  
  return 0;
}

int copy_to_enclave(int eid, uintptr_t encl_addr, uintptr_t ptr, size_t size)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;

  //printm("[sm] copy_to_enclave: eid[%d], va = 0x%llx, pa = 0x%llx, size = %d\n",eid, encl_addr, ptr, size);
  struct enclave_t encl = enclaves[eid];

  //void* epm = pmp_get_addr(encl.rid);
  
  //uintptr_t paddr = epm_alloc_page(&encl.epm, encl_addr);
  //TODO size is not always 4K. this code assumes 4K.
  //memcpy((void*)paddr, (void*)ptr, size);

  //debug dump
  /*for(int i=0; i<size; i++){
    printm("dump: 0x%.2x\n", ((char*)paddr)[i]);
  }*/

  return 0;
}

int copy_from_enclave(int eid, void* ptr, size_t size)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;

  struct enclave_t encl = enclaves[eid];
  void* epm = pmp_get_addr(encl.rid);

  //memcpy(ptr, epm, size);
  return 0;
}

reg run_enclave(int eid, uintptr_t ptr)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;
  
  //printm("ptr: %llx\n",ptr);

  struct enclave_t encl = enclaves[eid]; 

  enclaves[eid].host_mepc = read_csr(mepc);
  enclaves[eid].host_stvec = read_csr(stvec);
  write_csr(mepc, 0xffffffffc0000000 ); // address of trampoline (runtime)
  printm("enclave_satp = 0x%lx\n", encl.encl_satp);
  printm("host satp = 0x%lx\n",encl.host_satp);
  write_csr(satp, encl.encl_satp);
 
  // delegate user ecall (syscall) 
  //uintptr_t exceptions = read_csr(medeleg);
  //exceptions &= ~(1U << CAUSE_USER_ECALL);
  //write_csr(medeleg, exceptions);

  //disable timer interrupt
  clear_csr(mie, MIP_MTIP);
  pmp_unset(encl.rid);

  printm("entering enclave %d...\n",eid);  
  
  running_encl_id = eid;

  asm volatile(
      "mv tp, %0\n"
      "jal save_host_regs\n"
      : : "rK"(&encl.host_ctx));
/*  
  asm volatile(
      "csrrw sp, mscratch, sp\n"
      "mv a0, %0\n"
      "mret"
      : : "rK"(ptr));*/
  return (reg)ptr;
}

uint64_t exit_enclave(uint64_t retval)
{
  if(running_encl_id < 0)
    return -1;
  
  struct enclave_t encl = enclaves[running_encl_id];

  pmp_set(encl.rid);
  printm("exiting enclave %d...\n",running_encl_id);
  running_encl_id = -1;

  //uintptr_t exceptions = read_csr(medeleg);
  //exceptions |= (1U << CAUSE_USER_ECALL);
  //write_csr(medeleg, exceptions);
  write_csr(stvec, encl.host_stvec);
  set_csr(mie, MIP_MTIP);
  printm("mepc = 0x%lx\n",encl.host_mepc);
  write_csr(mepc, encl.host_mepc);
  printm("satp = 0x%lx\n",encl.host_satp);
  write_csr(satp, encl.host_satp);

//  asm volatile(
//      "csrrw sp, mscratch, sp\n"
//      "mret");
  return retval;
}


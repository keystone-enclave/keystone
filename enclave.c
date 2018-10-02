#include "bits.h"
#include "vm.h"
#include "enclave.h"
#include "pmp.h"
#include "page.h"
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

int detect_region_overlap(int eid, uintptr_t addr, uintptr_t size)
{
  void* epm_base;
  uint64_t epm_size;

  epm_base = pmp_get_addr(enclaves[eid].rid);
  epm_size = pmp_get_size(enclaves[eid].rid);

  return ((uintptr_t) epm_base < addr + size) &&
         ((uintptr_t) epm_base + epm_size > addr);
}

int init_enclave(uintptr_t base, uintptr_t size)
{
  int ret;
  int ptlevel = (VA_BITS - RISCV_PGSHIFT) / RISCV_PGLEVEL_BITS;
  
  // this function does the followings:
  // (1) Traverse the page table to see if any address points to the outside of EPM
  // (2) Zero out every page table entry that is not valid
  ret = init_encl_pgtable(ptlevel, (pte_t*) base, base, size);
  
  // FIXME: probably we will also need to:
  // (3) Zero out every page that is not pointed by the page table

  return ret;
}

int create_enclave(uintptr_t base, uintptr_t size)
{
  uint8_t perm = 0;
  int ret, region;
  int i;
  // - TODO: if size larger than minimum requirement (16 KB)
  // - if base and (base+size) not belong to other enclaves
  for(i=0; i<ENCL_MAX; i++)
  {
    if(!TEST_BIT(encl_bitmap, i))
      continue;
    if(detect_region_overlap(i, base, size))
    {
      printm("region overlaps with enclave %d\n", i);
      return -EINVAL;
    }
  }   

  // - if size is multiple of 4KB (smaller than 4KB not supported) 
  if(size & (RISCV_PGSIZE-1))
    return -EINVAL;

  // 1. create a PMP region binded to the enclave
  ret = pmp_region_init(base, size, perm);
  RET_ON_ERR(ret);

  region = ret;
  // 2. set pmp
  ret = pmp_set(region); 
  RET_ON_ERR(ret);
 
  // 3. initialize and verify enclave memory layout. 
  init_enclave(base, size);

  // 4. allocate eid
  ret = encl_alloc_idx();
  RET_ON_ERR(ret);

  // 5. initialize enclave metadata
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
  void* base = pmp_get_addr(enclaves[eid].rid);
  uintptr_t size = pmp_get_size(enclaves[eid].rid);
  memset((void*) base, 0, size);

  // 2. free pmp region
  pmp_region_free(enclaves[eid].rid);

  // 3. release eid
  encl_free_idx(eid);
  
  return 0;
}

int copy_to_enclave(int eid, uintptr_t encl_addr, uintptr_t ptr, size_t size)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;
  
  struct enclave_t encl = enclaves[eid];

  /* TODO: NOT IMPLEMENTED */

  return 0;
}

int copy_from_enclave(int eid, void* ptr, size_t size)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;

  struct enclave_t encl = enclaves[eid];
  void* epm = pmp_get_addr(encl.rid);

  /* TODO: NOT IMPLEMENTED */
  
  return 0;
}

reg run_enclave(int eid, uintptr_t ptr)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;
  struct enclave_t encl = enclaves[eid]; 

  // save host return pc
  enclaves[eid].host_mepc = read_csr(mepc);

  // save host interrupt handler
  enclaves[eid].host_stvec = read_csr(stvec);

  // entry point after return (mret)
  write_csr(mepc, 0xffffffffc0000000 ); // address of trampoline (runtime)

  // switch to enclave page table
  write_csr(satp, encl.encl_satp);
 
  // disable timer interrupt
  clear_csr(mie, MIP_MTIP);
  
  // unset PMP
  pmp_unset(encl.rid);

  // FIXME: this works for now 
  // because only one enclave will run on a SM.
  // We should make SM identify the enclave by using { encl_satp -> eid } map
  running_encl_id = eid;

  /*
  asm volatile(
      "mv tp, %0\n"
      "jal save_host_regs\n"
      : : "rK"(&encl.host_ctx));
  */

  // return enclave entry point (this is the first argument to the runtime)
  return (reg)ptr;
}

uint64_t exit_enclave(uint64_t retval)
{
  if(running_encl_id < 0)
    return -1;
 
  // get the running enclave on this SM 
  struct enclave_t encl = enclaves[running_encl_id];

  // set PMP
  pmp_set(encl.rid);
  running_encl_id = -1;

  // restore interrupt handler
  write_csr(stvec, encl.host_stvec);

  // enable timer interrupt
  set_csr(mie, MIP_MTIP);

  // restore host return pc
  write_csr(mepc, encl.host_mepc);

  // switch to host page table
  write_csr(satp, encl.host_satp);

  return retval;
}


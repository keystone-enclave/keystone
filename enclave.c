#include "enclave.h"
#include "pmp.h"
#include "page.h"
#include <string.h>

#define ENCL_MAX  16
static uint64_t encl_bitmap = 0;

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

  // 4. initialize page table
  // first page always be the top-level pt 
  unsigned int total_pages = (RISCV_PGSIZE + size - 1)>>RISCV_PGSHIFT;
  epm_init(&enclaves[ret].epm, base, total_pages);
  //epm->total = (PAGE_SIZE + size - 1) >> PAGE_SHIFT;
  //enclaves[ret].ptbr = ept_init(base);


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

  printm("[sm] copy_to_enclave: eid[%d], va = 0x%llx, pa = 0x%llx, size = %d\n",eid, encl_addr, ptr, size);
  struct enclave_t encl = enclaves[eid];

  //void* epm = pmp_get_addr(encl.rid);
  
  uintptr_t paddr = epm_alloc_page(&encl.epm, encl_addr);
  //TODO size is not always 4K. this code assumes 4K.
  memcpy((void*)paddr, (void*)ptr, size);

  //debug dump
  for(int i=0; i<size; i++){
    printm("dump: 0x%.2x\n", ((char*)paddr)[i]);
  }

  return 0;
}

int copy_from_enclave(int eid, void* ptr, size_t size)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;

  struct enclave_t encl = enclaves[eid];
  void* epm = pmp_get_addr(encl.rid);

  memcpy(ptr, epm, size);
  return 0;
}

int run_enclave(int eid, uintptr_t ptr)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;
  
  printm("ptr: %llx\n",ptr);

  struct enclave_t encl = enclaves[eid]; 
  encl.mepc = read_csr(mepc);
  printm("orig. empc: 0x%llx\n",encl.mepc);
  write_csr(mepc, ptr);
  write_csr(satp, epm_satp(&encl.epm));
  pmp_unset(encl.rid);
  printm("entering enclave...\n");
  asm volatile("csrrw sp, mscratch, sp\n"
      "ld a0, 10*(1<<3)(sp)\n"
      "ld a1, 10*(1<<3)(sp)\n"
      "mret"
      : :);
  return 0;
}

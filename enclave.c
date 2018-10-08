#include "bits.h"
#include "vm.h"
#include "enclave.h"
#include "pmp.h"
#include "page.h"
#include <string.h>
#include "atomic.h"

#define ENCL_MAX  16
#define RET_ON_ERR(ret) {if(ret<0) return ret;}

static uint64_t encl_bitmap = 0;

struct enclave_t enclaves[ENCL_MAX];

static spinlock_t encl_lock = SPINLOCK_INIT;

/* FIXME: this takes O(n), change it to use a hash table */
int encl_satp_to_eid(reg satp)
{
  int i;
  for(i=0; i<ENCL_MAX; i++)
  {
    if(enclaves[i].encl_satp == satp)
      return i;
  }
  return -1;
}
/* FIXME: this takes O(n), change it to use a hash table */
int host_satp_to_eid(reg satp)
{
  int i;
  for(i=0; i<ENCL_MAX; i++)
  {
    if(enclaves[i].host_satp == satp)
      return i;
  }
  return -1;
}

int encl_alloc_idx()
{
  int i;
  
  spinlock_lock(&encl_lock);
  
  for(i=0; i<ENCL_MAX; i++)
  {
    if(!(encl_bitmap & (0x1 << i)))
      break;
  }
  if(i != ENCL_MAX)
    SET_BIT(encl_bitmap, i);

  spinlock_unlock(&encl_lock);

  if(i != ENCL_MAX)
    return i;
  else
    return -1;  
}

int encl_free_idx(int idx)
{
  spinlock_lock(&encl_lock);
  UNSET_BIT(encl_bitmap, idx);
  spinlock_unlock(&encl_lock);
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

int init_enclave_memory(uintptr_t base, uintptr_t size)
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
  int eid;
  int ret, region;
  int i;
  int region_overlap = 0;
  
  // 1. create a PMP region binded to the enclave
  ret = pmp_region_init_atomic(base, size, perm, PMP_PRI_ANY);
  if(ret < 0)
    goto error;
  region = ret;

  // - if base and (base+size) not belong to other enclaves
  spinlock_lock(&encl_lock);
  for(i=0; i<ENCL_MAX; i++)
  {
    if(!TEST_BIT(encl_bitmap, i))
      continue;
    region_overlap |= detect_region_overlap(i, base, size);
    if(region_overlap)
      break;
  }
  spinlock_unlock(&encl_lock);

  if(region_overlap)
  {
    printm("region overlaps with enclave %d\n", i);
    ret = -EINVAL;
    goto free_region;
  }

  // 2. allocate eid
  ret = encl_alloc_idx();
  if(ret < 0)
    goto free_region;
  eid = ret;

  // 3. set pmp
  ret = pmp_set_global(region); 
  //ret = pmp_set(region);
  if(ret < 0)
    goto free_encl_idx;

  
  // 4. initialize and verify enclave memory layout. 
  init_enclave_memory(base, size);

  // 5. initialize enclave metadata
  enclaves[eid].eid = eid;
  enclaves[eid].rid = region;
  enclaves[eid].host_satp = read_csr(satp);
  enclaves[eid].encl_satp = ((base >> RISCV_PGSHIFT) | SATP_MODE_CHOICE);
  enclaves[eid].n_thread = 0;

  spinlock_lock(&encl_lock);
  enclaves[eid].state = INITIALIZED;
  spinlock_unlock(&encl_lock);
  
  return 0;
 
free_encl_idx:
  encl_free_idx(eid);
free_region:
  pmp_region_free_atomic(region);
error:
  return ret;
}

int destroy_enclave(int eid)
{
  int destroyable;

  spinlock_lock(&encl_lock);
  destroyable = TEST_BIT(encl_bitmap, eid) && 
                (enclaves[eid].state >= 0) && 
                enclaves[eid].state != RUNNING;
  /* update the enclave state first so that
   * no SM can run the enclave any longer */
  if(destroyable)
    enclaves[eid].state = DESTROYED;
  spinlock_unlock(&encl_lock);

  if(!destroyable)
    return -1;
  
  // 1. clear all the data in the enclave page
  // requires no lock (single runner)
  void* base = pmp_get_addr(enclaves[eid].rid);
  uintptr_t size = pmp_get_size(enclaves[eid].rid);
  //memset((void*) base, 0, size);

  // 2. free pmp region
  pmp_unset_global(enclaves[eid].rid);
  pmp_region_free_atomic(enclaves[eid].rid);

  enclaves[eid].eid = 0;
  enclaves[eid].rid = 0;
  enclaves[eid].host_satp = 0;
  enclaves[eid].encl_satp = 0;
  enclaves[eid].n_thread = 0;

  // 3. release eid
  encl_free_idx(eid);
  
  return 0;
}

reg run_enclave(int eid, uintptr_t ptr)
{
  int runable;
  int hart_id;

  spinlock_lock(&encl_lock);
  runable = TEST_BIT(encl_bitmap, eid) && (enclaves[eid].state >= 0);
  if(runable) {
    enclaves[eid].state = RUNNING;
    enclaves[eid].n_thread++;
  }
  spinlock_unlock(&encl_lock);

  if(!runable)
    return -1;

  hart_id = read_csr(mhartid);
  
  /* save host context */
  enclaves[eid].host_mepc[hart_id] = read_csr(mepc);
  enclaves[eid].host_stvec[hart_id] = read_csr(stvec);

  // entry point after return (mret)
  write_csr(mepc, 0xffffffffc0000000 ); // address of trampoline (runtime)

  // switch to enclave page table
  write_csr(satp, enclaves[eid].encl_satp);
 
  // disable timer interrupt
  clear_csr(mie, MIP_MTIP);
  
  // unset PMP
  pmp_unset(enclaves[eid].rid);

  // return enclave entry point (this is the first argument to the runtime)
  return (reg)ptr;
}

uint64_t exit_enclave(uint64_t retval)
{
  int eid = encl_satp_to_eid(read_csr(satp));
  int exitable;
  int hart_id = read_csr(mhartid);

  if(eid < 0)
    return -1;
 
  spinlock_lock(&encl_lock);
  exitable = enclaves[eid].state == RUNNING;
  spinlock_unlock(&encl_lock);

  if(!exitable)
    return -1;

  // get the running enclave on this SM 
  struct enclave_t encl = enclaves[eid];

  // set PMP
  pmp_set(encl.rid);

  /* restore host context */
  write_csr(stvec, encl.host_stvec[hart_id]);
  write_csr(mepc, encl.host_mepc[hart_id]);

  // switch to host page table
  write_csr(satp, encl.host_satp);

  // enable timer interrupt
  set_csr(mie, MIP_MTIP);

  // update enclave state
  spinlock_lock(&encl_lock);
  enclaves[eid].n_thread--;
  if(enclaves[eid].n_thread == 0)
    enclaves[eid].state = INITIALIZED;
  spinlock_unlock(&encl_lock);

  return retval;
}


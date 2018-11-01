#include "sm.h"
#include "bits.h"
#include "vm.h"
#include "enclave.h"
#include "pmp.h"
#include "page.h"
#include <string.h>
#include "atomic.h"

#define ENCL_MAX  16

static uint64_t encl_bitmap = 0;

struct enclave_t enclaves[ENCL_MAX];

static spinlock_t encl_lock = SPINLOCK_INIT;

extern void save_host_regs(void);
extern void restore_host_regs(void);

/* FIXME: this takes O(n), change it to use a hash table */
int encl_satp_to_eid(uintptr_t satp, unsigned int* eid)
{
  unsigned int i;
  for(i=0; i<ENCL_MAX; i++)
  {
    if(enclaves[i].encl_satp == satp){
      *eid = i;
      return i;
    }
  }
  return -1;
}
/* FIXME: this takes O(n), change it to use a hash table */
int host_satp_to_eid(uintptr_t satp, unsigned int* eid)
{
  unsigned int i;
  for(i=0; i<ENCL_MAX; i++)
  {
    if(enclaves[i].host_satp == satp){
      *eid = i;
      return i;
    }
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

unsigned long get_host_satp(unsigned int eid)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;

  return enclaves[eid].host_satp;
}

int detect_region_overlap(unsigned int eid, uintptr_t addr, uintptr_t size)
{
  void* epm_base;
  size_t epm_size;

  epm_base = pmp_get_addr(enclaves[eid].rid);
  epm_size = pmp_get_size(enclaves[eid].rid);

  return ((uintptr_t) epm_base < addr + size) &&
         ((uintptr_t) epm_base + epm_size > addr);
}

void copy_word_to_host(uintptr_t* ptr, uintptr_t value)
{
  int region_overlap = 0, i;
  spinlock_lock(&encl_lock);
  for(i=0; i<ENCL_MAX; i++)
  {
    if(!TEST_BIT(encl_bitmap, i))
      continue;
    region_overlap |= detect_region_overlap(i, (uintptr_t) ptr, sizeof(uintptr_t));
    if(region_overlap)
      break;
  }
  if(!region_overlap)
    *ptr = value;
  else
    *ptr = -1UL;
  spinlock_unlock(&encl_lock);
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

enclave_ret_t create_enclave(uintptr_t base, uintptr_t size, unsigned int* eidptr)
{
  uint8_t perm = 0;
  unsigned int eid;
  enclave_ret_t ret;
  int region;
  int i;
  int region_overlap = 0;
  
  // 1. create a PMP region binded to the enclave
  
  ret = ENCLAVE_PMP_FAILURE;

  if(pmp_region_init_atomic(base, size, PMP_PRI_ANY, &region))
    goto error;

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
    goto free_region;
  }

  // 2. allocate eid
  eid = encl_alloc_idx();
  if(eid < 0)
    goto free_region;

  // 3. set pmp
  if(pmp_set_global(region, PMP_NO_PERM))
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
 
  copy_word_to_host((uintptr_t*)eidptr, (uintptr_t)eid);

  return ENCLAVE_SUCCESS;
 
free_encl_idx:
  encl_free_idx(eid);
free_region:
  pmp_region_free_atomic(region);
error:
  return ret;
}

enclave_ret_t destroy_enclave(unsigned int eid)
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
    return ENCLAVE_NOT_DESTROYABLE;
  
  // 1. clear all the data in the enclave page
  // requires no lock (single runner)
  void* base = pmp_get_addr(enclaves[eid].rid);
  size_t size = pmp_get_size(enclaves[eid].rid);
  memset((void*) base, 0, size);

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
  
  return ENCLAVE_SUCCESS;
}

#define RUNTIME_START_ADDRESS 0xffffffffc0000000

enclave_ret_t run_enclave(uintptr_t* host_regs, unsigned int eid, uintptr_t entry, unsigned long* retptr)
{
  int runable;

  spinlock_lock(&encl_lock);
  runable = TEST_BIT(encl_bitmap, eid) 
    && (enclaves[eid].state >= 0) 
    && enclaves[eid].n_thread < MAX_ENCL_THREADS;
  if(runable) {
    enclaves[eid].state = RUNNING;
    enclaves[eid].n_thread++;
  }
  spinlock_unlock(&encl_lock);

  if(!runable) {
    return ENCLAVE_NOT_RUNNABLE;
  }

  /* check if the entry point is valid */
  if(entry >= RUNTIME_START_ADDRESS)
  {
    return ENCLAVE_ILLEGAL_ARGUMENT;
  }
  /* TODO: only one thread is supported */
  set_retptr(&enclaves[eid].threads[0], retptr);

  /* save host context */
  swap_prev_state(&enclaves[eid].threads[0], host_regs);
  swap_prev_mepc(&enclaves[eid].threads[0], read_csr(mepc)); 
  swap_prev_stvec(&enclaves[eid].threads[0], read_csr(stvec));

  // entry point after return (mret)
  write_csr(mepc, RUNTIME_START_ADDRESS); // address of trampoline (runtime)

  // switch to enclave page table
  write_csr(satp, enclaves[eid].encl_satp);
 
  // disable timer set by the OS, clear pending interrupts
  clear_csr(mie, MIP_MTIP);

  clear_csr(mip, MIP_MTIP);
  clear_csr(mip, MIP_STIP);
  clear_csr(mip, MIP_SSIP);
  clear_csr(mip, MIP_SEIP);

  // set PMP
  pmp_set(enclaves[eid].rid, PMP_ALL_PERM);
  osm_pmp_set(PMP_NO_PERM);

  return ENCLAVE_SUCCESS;
}

enclave_ret_t exit_enclave(uintptr_t* encl_regs, unsigned long retval)
{
  unsigned int eid;
  int exitable;

  if(encl_satp_to_eid(read_csr(satp),&eid) < 0)
    return ENCLAVE_INVALID_ID;
 
  spinlock_lock(&encl_lock);
  exitable = enclaves[eid].state == RUNNING;
  spinlock_unlock(&encl_lock);

  if(!exitable)
    return ENCLAVE_NOT_RUNNING;
  
  // get the running enclave on this SM 
  struct enclave_t encl = enclaves[eid];

  // set PMP
  pmp_set(encl.rid, PMP_NO_PERM);
  osm_pmp_set(PMP_ALL_PERM);

  // copy return value to host
  copy_word_to_host(encl.threads[0].retptr, retval);
  /* restore host context */
  swap_prev_state(&enclaves[eid].threads[0], encl_regs);
  swap_prev_stvec(&enclaves[eid].threads[0], 0);
  swap_prev_mepc(&enclaves[eid].threads[0], 0); 

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

  return ENCLAVE_SUCCESS;
}

enclave_ret_t stop_enclave(uintptr_t* encl_regs, uint64_t request)
{
  unsigned int eid;
  int stoppable;
  if(encl_satp_to_eid(read_csr(satp),&eid) < 0)
    return ENCLAVE_INVALID_ID;

  spinlock_lock(&encl_lock);
  stoppable = enclaves[eid].state == RUNNING;

  spinlock_unlock(&encl_lock);

  if(!stoppable)
    return ENCLAVE_NOT_RUNNING;

  /* TODO: currently enclave cannot have multiple threads */
  swap_prev_state(&enclaves[eid].threads[0], encl_regs);
  swap_prev_mepc(&enclaves[eid].threads[0], read_csr(mepc));
  swap_prev_stvec(&enclaves[eid].threads[0], read_csr(stvec));  
  struct enclave_t encl = enclaves[eid];
  
  pmp_set(encl.rid, PMP_NO_PERM);
  osm_pmp_set(PMP_ALL_PERM);

  write_csr(satp, encl.host_satp);
  set_csr(mie, MIP_MTIP);
  
  return ENCLAVE_INTERRUPTED; 
}

enclave_ret_t resume_enclave(uintptr_t* host_regs, unsigned int eid)
{
  int resumable;

  spinlock_lock(&encl_lock);
  resumable = TEST_BIT(encl_bitmap, eid) 
    && (enclaves[eid].state == RUNNING) // not necessary 
    && enclaves[eid].n_thread > 0; // not necessary
  spinlock_unlock(&encl_lock);

  if(!resumable) {
    return ENCLAVE_NOT_RESUMABLE;
  }

  /* save host context */
  swap_prev_state(&enclaves[eid].threads[0], host_regs);
  swap_prev_mepc(&enclaves[eid].threads[0], read_csr(mepc)); 
  swap_prev_stvec(&enclaves[eid].threads[0], read_csr(stvec));

  // switch to enclave page table
  write_csr(satp, enclaves[eid].encl_satp);
 
  // disable timer set by the OS 
  clear_csr(mie, MIP_MTIP);

  clear_csr(mip, MIP_MTIP);
  clear_csr(mip, MIP_STIP);
  clear_csr(mip, MIP_SSIP);
  clear_csr(mip, MIP_SEIP);

  // set PMP
  pmp_set(enclaves[eid].rid, PMP_ALL_PERM);
  osm_pmp_set(PMP_NO_PERM); 

  return ENCLAVE_SUCCESS;
}

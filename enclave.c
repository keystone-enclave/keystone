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

int copy_word_to_host(uintptr_t* ptr, uintptr_t value)
{
  int region_overlap = 0, i;
  spinlock_lock(&encl_lock);
  region_overlap = detect_region_overlap_atomic((uintptr_t)ptr, sizeof(uintptr_t));
  if(!region_overlap)
    *ptr = value;
  spinlock_unlock(&encl_lock);

  if(region_overlap)
    return ENCLAVE_REGION_OVERLAPS;
  else
    return ENCLAVE_SUCCESS;
}


// Does not do checking of dest!
int copy_region_from_host(void* source, void* dest, size_t size){

  int region_overlap = 0, i;
  spinlock_lock(&encl_lock);
  region_overlap = detect_region_overlap_atomic((uintptr_t) source, sizeof(uintptr_t));
  if(!region_overlap)
    memcpy(dest, source, size);
  spinlock_unlock(&encl_lock);

  if(region_overlap)
    return ENCLAVE_REGION_OVERLAPS;
  else
    return ENCLAVE_SUCCESS;
  
}

int init_enclave_memory(uintptr_t base, uintptr_t size, uintptr_t utbase, uintptr_t utsize)
{
  int ret;
  int ptlevel = RISCV_PGLEVEL_TOP;
  
  // this function does the followings:
  // (1) Traverse the page table to see if any address points to the outside of EPM
  // (2) Zero out every page table entry that is not valid
  ret = init_encl_pgtable(ptlevel, (pte_t*) base, base, size, utbase, utsize);
  
  // FIXME: probably we will also need to:
  // (3) Zero out every page that is not pointed by the page table

  return ret;
}
enclave_ret_t create_enclave(struct keystone_sbi_create_t create_args)
{
  uintptr_t base = create_args.epm_region.paddr;
  size_t size = create_args.epm_region.size;
  uintptr_t utbase = create_args.utm_region.paddr;
  size_t utsize = create_args.utm_region.size;
  unsigned int* eidptr = create_args.eid_pptr;
    
  uint8_t perm = 0;
  unsigned int eid;
  enclave_ret_t ret;
  int region, shared_region;
  int i;
  int region_overlap = 0;
  
  // 1. create a PMP region binded to the enclave
  ret = ENCLAVE_PMP_FAILURE;

  if(pmp_region_init_atomic(base, size, PMP_PRI_ANY, &region, 0))
    goto error;

  if(pmp_region_init_atomic(utbase, utsize, PMP_PRI_BOTTOM, &shared_region, 0))
    goto free_region;

  // 2. allocate eid
  eid = encl_alloc_idx();
  if(eid < 0)
    goto free_shared_region;

  // 3. set pmp
  if(pmp_set_global(region, PMP_NO_PERM))
    goto free_encl_idx;
  
  // 4. initialize and verify enclave memory layout. 
  init_enclave_memory(base, size, utbase, utsize);

  // 5. initialize enclave metadata
  enclaves[eid].eid = eid;
  enclaves[eid].rid = region;
  enclaves[eid].utrid = shared_region;
  enclaves[eid].host_satp = read_csr(satp);
  //print_pgtable(3, (pte_t*) (read_csr(satp) << RISCV_PGSHIFT), 0);
  enclaves[eid].encl_satp = ((base >> RISCV_PGSHIFT) | SATP_MODE_CHOICE);
  enclaves[eid].n_thread = 0;
  enclaves[eid].enclave_entry = create_args.enclave_entry;
  enclaves[eid].runtime_entry = create_args.runtime_entry;

  /* prepare hash and signature for attestation */
  spinlock_lock(&encl_lock);
  enclaves[eid].state = FRESH;
  hash_enclave(&enclaves[eid]);
  sign_enclave(&enclaves[eid]);
  spinlock_unlock(&encl_lock);
 
  copy_word_to_host((uintptr_t*)eidptr, (uintptr_t)eid);

  return ENCLAVE_SUCCESS;
 
free_encl_idx:
  encl_free_idx(eid);
free_shared_region:
  pmp_region_free_atomic(shared_region);
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
  pmp_region_free_atomic(enclaves[eid].utrid);

  enclaves[eid].eid = 0;
  enclaves[eid].rid = 0;
  enclaves[eid].utrid = 0;
  enclaves[eid].host_satp = 0;
  enclaves[eid].encl_satp = 0;
  enclaves[eid].n_thread = 0;
  enclaves[eid].enclave_entry = 0;
  enclaves[eid].runtime_entry = 0;

  // 3. release eid
  encl_free_idx(eid);
  
  return ENCLAVE_SUCCESS;
}

enclave_ret_t run_enclave(uintptr_t* host_regs, unsigned int eid)
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

  /* save host context */
  swap_prev_state(&enclaves[eid].threads[0], host_regs);

  /* Swapping the mepc sets up the mret in mtrap.c to transfer control
     to the enclave */
  swap_prev_mepc(&enclaves[eid].threads[0], read_csr(mepc)); 
  swap_prev_stvec(&enclaves[eid].threads[0], read_csr(stvec));


  // entry points
  write_csr(mepc, enclaves[eid].runtime_entry);
  host_regs[11] = enclaves[eid].enclave_entry;
  
  // switch to enclave page table
  write_csr(satp, enclaves[eid].encl_satp);
 
  // disable timer set by the OS, clear pending interrupts
  clear_csr(mie, MIP_MTIP);
  clear_csr(mip, MIP_MSIP);
  clear_csr(mip, MIP_STIP);
  clear_csr(mip, MIP_SSIP);

  clear_csr(mip, MIP_MTIP);
  clear_csr(mip, MIP_STIP);
  clear_csr(mip, MIP_SSIP);
  clear_csr(mip, MIP_SEIP);

  // set PMP
  pmp_set(enclaves[eid].rid, PMP_ALL_PERM);
  osm_pmp_set(PMP_NO_PERM);
  pmp_set(enclaves[eid].utrid, PMP_ALL_PERM);

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
 
  switch(request) {
    case(STOP_TIMER_INTERRUPT):
      return ENCLAVE_INTERRUPTED;
    case(STOP_EDGE_CALL_HOST):
      return ENCLAVE_EDGE_CALL_HOST;
    default:
      return ENCLAVE_UNKNOWN_ERROR;
  }
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
  pmp_set(enclaves[eid].utrid, PMP_ALL_PERM);

  return ENCLAVE_SUCCESS;
}

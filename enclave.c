//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "sm.h"
#include "bits.h"
#include "vm.h"
#include "enclave.h"
#include "pmp.h"
#include "page.h"
#include <string.h>
#include "atomic.h"

#define ENCL_MAX  16

struct enclave_t enclaves[ENCL_MAX];
#define ENCLAVE_EXISTS(eid) (enclaves[eid].state >= 0)

static spinlock_t encl_lock = SPINLOCK_INIT;

extern void save_host_regs(void);
extern void restore_host_regs(void);
extern byte dev_public_key[PUBLIC_KEY_SIZE];

/*********************************
 *
 * Enclave SBI functions
 * These are exposed to S-mode via the sm-sbi interface
 *
 *********************************/


/* This handles creation of a new enclave, based on arguments provided
 * by the untrusted host.
 *
 * This may fail if: it cannot allocate PMP regions, EIDs, etc
 */
enclave_ret_t create_enclave(struct keystone_sbi_create_t create_args)
{
  /* EPM and UTM parameters */
  uintptr_t base = create_args.epm_region.paddr;
  size_t size = create_args.epm_region.size;
  uintptr_t utbase = create_args.utm_region.paddr;
  size_t utsize = create_args.utm_region.size;
  eid_t* eidptr = create_args.eid_pptr;

  /* Runtime parameters */
  struct runtime_params_t params = create_args.params;

  uint8_t perm = 0;
  eid_t eid;
  enclave_ret_t ret;
  int region, shared_region;
  int i;
  int region_overlap = 0;

  // allocate eid
  ret = ENCLAVE_NO_FREE_RESOURCE;
  if(encl_alloc_eid(&eid) != ENCLAVE_SUCCESS)
    goto error;

  // create a PMP region bound to the enclave
  ret = ENCLAVE_PMP_FAILURE;
  if(pmp_region_init_atomic(base, size, PMP_PRI_ANY, &region, 0))
    goto free_encl_idx;

  // create PMP region for shared memory
  if(pmp_region_init_atomic(utbase, utsize, PMP_PRI_BOTTOM, &shared_region, 0))
    goto free_region;

  // set pmp registers for private region (not shared)
  if(pmp_set_global(region, PMP_NO_PERM))
    goto free_shared_region;

  // initialize and verify enclave memory layout.
  init_enclave_memory(base, size, utbase, utsize);

  // initialize enclave metadata
  enclaves[eid].eid = eid;
  enclaves[eid].rid = region;
  enclaves[eid].utrid = shared_region;
  enclaves[eid].host_satp = read_csr(satp);
  //print_pgtable(3, (pte_t*) (read_csr(satp) << RISCV_PGSHIFT), 0);
  enclaves[eid].encl_satp = ((base >> RISCV_PGSHIFT) | SATP_MODE_CHOICE);
  enclaves[eid].n_thread = 0;
  enclaves[eid].params = params;

  /* prepare hash and signature for attestation */
  spinlock_lock(&encl_lock);
  enclaves[eid].state = FRESH;
  hash_enclave(&enclaves[eid]);
  spinlock_unlock(&encl_lock);

  /* EIDs are unsigned int in size, copy via simple copy */
  copy_word_to_host((uintptr_t*)eidptr, (uintptr_t)eid);

  return ENCLAVE_SUCCESS;

free_shared_region:
  pmp_region_free_atomic(shared_region);
free_region:
  pmp_region_free_atomic(region);
free_encl_idx:
  encl_free_eid(eid);
error:
  return ret;
}

/*
 * Fully destroys an enclave
 * Deallocates EID, clears epm, etc
 * Fails only if the enclave isn't running.
 */
enclave_ret_t destroy_enclave(eid_t eid)
{
  int destroyable;

  spinlock_lock(&encl_lock);
  destroyable = (ENCLAVE_EXISTS(eid)
                 && enclaves[eid].state != RUNNING
                 && enclaves[eid].state != ALLOCATED);
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
  enclaves[eid].params = (struct runtime_params_t) {0};

  // 3. release eid
  encl_free_eid(eid);

  return ENCLAVE_SUCCESS;
}

enclave_ret_t run_enclave(uintptr_t* host_regs, eid_t eid)
{
  int runable;

  spinlock_lock(&encl_lock);
  runable = (ENCLAVE_EXISTS(eid)
             && enclaves[eid].n_thread < MAX_ENCL_THREADS);
  if(runable) {
    enclaves[eid].state = RUNNING;
    enclaves[eid].n_thread++;
  }
  spinlock_unlock(&encl_lock);

  if(!runable) {
    return ENCLAVE_NOT_RUNNABLE;
  }

  // Enclave is OK to run, context switch to it
  return _context_switch_to_enclave(host_regs, eid, 1);
}

enclave_ret_t exit_enclave(uintptr_t* encl_regs, unsigned long retval)
{
  eid_t eid;
  int exitable;

  if(encl_satp_to_eid(read_csr(satp),&eid) != ENCLAVE_SUCCESS)
    return ENCLAVE_INVALID_ID;

  spinlock_lock(&encl_lock);
  exitable = enclaves[eid].state == RUNNING;
  spinlock_unlock(&encl_lock);

  if(!exitable)
    return ENCLAVE_NOT_RUNNING;

  _context_switch_to_host(encl_regs, eid);

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
  eid_t eid;
  int stoppable;
  if(encl_satp_to_eid(read_csr(satp),&eid) != ENCLAVE_SUCCESS)
    return ENCLAVE_INVALID_ID;

  spinlock_lock(&encl_lock);
  stoppable = enclaves[eid].state == RUNNING;
  spinlock_unlock(&encl_lock);

  if(!stoppable)
    return ENCLAVE_NOT_RUNNING;

  _context_switch_to_host(encl_regs, eid);

  switch(request) {
  case(STOP_TIMER_INTERRUPT):
    return ENCLAVE_INTERRUPTED;
  case(STOP_EDGE_CALL_HOST):
    return ENCLAVE_EDGE_CALL_HOST;
  default:
    return ENCLAVE_UNKNOWN_ERROR;
  }


}


enclave_ret_t resume_enclave(uintptr_t* host_regs, eid_t eid)
{
  int resumable;

  spinlock_lock(&encl_lock);
  resumable = (ENCLAVE_EXISTS(eid)
               && (enclaves[eid].state == RUNNING) // not necessary
               && enclaves[eid].n_thread > 0); // not necessary
  spinlock_unlock(&encl_lock);

  if(!resumable) {
    return ENCLAVE_NOT_RESUMABLE;
  }

  // Enclave is OK to resume, context switch to it
  return _context_switch_to_enclave(host_regs, eid, 0);
}

enclave_ret_t attest_enclave(uintptr_t report_ptr, uintptr_t data, uintptr_t size)
{
  int attestable;
  struct report_t report;
  int ret;
  eid_t eid;
  if(encl_satp_to_eid(read_csr(satp),&eid) != ENCLAVE_SUCCESS)
    return ENCLAVE_INVALID_ID;

  if (size > ATTEST_DATA_MAXLEN)
    return ENCLAVE_ILLEGAL_ARGUMENT;

  spinlock_lock(&encl_lock);
  attestable = (ENCLAVE_EXISTS(eid)
                && (enclaves[eid].state >= INITIALIZED));
  spinlock_unlock(&encl_lock);

  if(!attestable)
    return ENCLAVE_NOT_INITIALIZED;

  /* copy data to be signed */
  ret = copy_from_enclave(&enclaves[eid],
      report.enclave.data,
      (void*) get_phys_addr(data),
      size);
  report.enclave.data_len = size;

  if (ret) {
    return ret;
  }

  memcpy(report.dev_public_key, dev_public_key, PUBLIC_KEY_SIZE);
  memcpy(report.sm.hash, sm_hash, MDSIZE);
  memcpy(report.sm.public_key, sm_public_key, PUBLIC_KEY_SIZE);
  memcpy(report.sm.signature, sm_signature, SIGNATURE_SIZE);
  memcpy(report.enclave.hash, enclaves[eid].hash, MDSIZE);
  sm_sign(report.enclave.signature,
      &report.enclave,
      sizeof(struct enclave_report_t)
      - SIGNATURE_SIZE
      - ATTEST_DATA_MAXLEN + size);

  /* copy report to the enclave */
  ret = copy_to_enclave(&enclaves[eid],
      (void*) get_phys_addr(report_ptr),
      &report,
      sizeof(struct report_t));
  if (ret) {
    return ret;
  }

  return ENCLAVE_SUCCESS;
}

/****************************
 *
 * Enclave utility functions
 * Internal use by above SBI calls
 *
 ****************************/

/* Internal function containing the core of the context switching
 * code to the enclave.
 *
 * Used by resume_enclave and run_enclave.
 *
 * Expects that eid has already been valided, and it is OK to run this enclave
*/
inline enclave_ret_t _context_switch_to_enclave(uintptr_t* regs,
                                                eid_t eid,
                                                int load_parameters){

  /* save host context */
  swap_prev_state(&enclaves[eid].threads[0], regs);
  swap_prev_mepc(&enclaves[eid].threads[0], read_csr(mepc));
  swap_prev_stvec(&enclaves[eid].threads[0], read_csr(stvec));

  if(load_parameters){
    // passing parameters for a first run
    write_csr(mepc, (uintptr_t) enclaves[eid].params.runtime_entry);
    regs[11] = (uintptr_t) enclaves[eid].params.user_entry;
    regs[12] = (uintptr_t) enclaves[eid].params.untrusted_ptr;
    regs[13] = (uintptr_t) enclaves[eid].params.untrusted_size;
  }

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

inline void _context_switch_to_host(uintptr_t* encl_regs,
                                    eid_t eid){
  // get the running enclave on this SM
  struct enclave_t encl = enclaves[eid];

  // set PMP
  pmp_set(encl.rid, PMP_NO_PERM);
  osm_pmp_set(PMP_ALL_PERM);

  /* restore host context */
  swap_prev_state(&enclaves[eid].threads[0], encl_regs);
  swap_prev_stvec(&enclaves[eid].threads[0], read_csr(stvec));
  swap_prev_mepc(&enclaves[eid].threads[0], read_csr(mepc));

  // switch to host page table
  write_csr(satp, encl.host_satp);

  // enable timer interrupt
  set_csr(mie, MIP_MTIP);

  return;
}


/*
 * Init all metadata as needed for keeping track of enclaves
 * Called once by the SM on startup
 */
void enclave_init_metadata(){
  eid_t eid;

  /* Assumes eids are incrementing values, which they are for now */
  for(eid=0; eid < ENCL_MAX; eid++){
    enclaves[eid].state = INVALID;
  }
}


enclave_ret_t init_enclave_memory(uintptr_t base, uintptr_t size,
                                  uintptr_t utbase, uintptr_t utsize)
{
  int ret;
  int ptlevel = RISCV_PGLEVEL_TOP;

  // this function does the followings:
  // (1) Traverse the page table to see if any address points to the outside of EPM
  // (2) Zero out every page table entry that is not valid
  ret = init_encl_pgtable(ptlevel, (pte_t*) base, base, size, utbase, utsize);

  // FIXME: probably we will also need to:
  // (3) Zero out every page that is not pointed by the page table

  // Zero out the untrusted memory region, since it may be in
  // indeterminate state.
  memset((void*)utbase, 0, utsize);

  return ret;
}

/* FIXME: this takes O(n), change it to use a hash table */
enclave_ret_t encl_satp_to_eid(uintptr_t satp, eid_t* eid)
{
  unsigned int i;
  for(i=0; i<ENCL_MAX; i++)
  {
    if(enclaves[i].encl_satp == satp){
      *eid = i;
      return ENCLAVE_SUCCESS;
    }
  }
  return ENCLAVE_INVALID_ID;
}
/* FIXME: this takes O(n), change it to use a hash table */
enclave_ret_t host_satp_to_eid(uintptr_t satp, eid_t* eid)
{
  unsigned int i;
  for(i=0; i<ENCL_MAX; i++)
  {
    if(enclaves[i].host_satp == satp){
      *eid = i;
      return ENCLAVE_SUCCESS;
    }
  }
  return ENCLAVE_INVALID_ID;
}

enclave_ret_t encl_alloc_eid(eid_t* _eid)
{
  eid_t eid;

  spinlock_lock(&encl_lock);

  for(eid=0; eid<ENCL_MAX; eid++)
  {
    if(enclaves[eid].state < 0){
      break;
    }
  }
  if(eid != ENCL_MAX)
    enclaves[eid].state = ALLOCATED;

  spinlock_unlock(&encl_lock);

  if(eid != ENCL_MAX){
    *_eid = eid;
    return ENCLAVE_SUCCESS;
  }
  else{
    return ENCLAVE_NO_FREE_RESOURCE;
  }
}

enclave_ret_t encl_free_eid(eid_t eid)
{
  spinlock_lock(&encl_lock);
  enclaves[eid].state = DESTROYED;
  spinlock_unlock(&encl_lock);
  return ENCLAVE_SUCCESS;
}

enclave_ret_t get_host_satp(eid_t eid, unsigned long* satp)
{
  if(!ENCLAVE_EXISTS(eid))
    return ENCLAVE_NOT_ACCESSIBLE;

  *satp = enclaves[eid].host_satp;

  return ENCLAVE_SUCCESS;
}

/* Ensures that dest ptr is in host, not in enclave regions
 */
enclave_ret_t copy_word_to_host(uintptr_t* dest_ptr, uintptr_t value)
{
  int region_overlap = 0;
  spinlock_lock(&encl_lock);
  region_overlap = detect_region_overlap_atomic((uintptr_t)dest_ptr,
                                                sizeof(uintptr_t));
  if(!region_overlap)
    *dest_ptr = value;
  spinlock_unlock(&encl_lock);

  if(region_overlap)
    return ENCLAVE_REGION_OVERLAPS;
  else
    return ENCLAVE_SUCCESS;
}

/* Internal function enforcing a copy source is from the untrusted world.
 * Does NOT do verification of dest, assumes caller knows what that is.
 * Dest should be inside the SM memory.
 */
enclave_ret_t copy_from_host(void* source, void* dest, size_t size){

  int region_overlap = 0;
  spinlock_lock(&encl_lock);
  region_overlap = detect_region_overlap_atomic((uintptr_t) source, size);
  // TODO: Validate that dest is inside the SM.
  if(!region_overlap)
    memcpy(dest, source, size);
  spinlock_unlock(&encl_lock);

  if(region_overlap)
    return ENCLAVE_REGION_OVERLAPS;
  else
    return ENCLAVE_SUCCESS;
}

/* copies data from enclave, source must be inside EPM */
enclave_ret_t copy_from_enclave(struct enclave_t* enclave,
                                void* dest, void* source, size_t size) {
  int legal = 0;
  spinlock_lock(&encl_lock);
  legal = (source >= pmp_get_addr(enclave->rid)
      && source + size <= pmp_get_addr(enclave->rid) +
      pmp_get_size(enclave->rid));
  if(legal)
    memcpy(dest, source, size);
  spinlock_unlock(&encl_lock);

  if(!legal)
    return ENCLAVE_ILLEGAL_ARGUMENT;
  else
    return ENCLAVE_SUCCESS;
}

/* copies data into enclave, destination must be inside EPM */
enclave_ret_t copy_to_enclave(struct enclave_t* enclave,
                              void* dest, void* source, size_t size) {
  int legal = 0;
  spinlock_lock(&encl_lock);
  legal = (dest >= pmp_get_addr(enclave->rid)
      && dest + size <= pmp_get_addr(enclave->rid) +
      pmp_get_size(enclave->rid));
  if(legal)
    memcpy(dest, source, size);
  spinlock_unlock(&encl_lock);

  if(!legal)
    return ENCLAVE_ILLEGAL_ARGUMENT;
  else
    return ENCLAVE_SUCCESS;
}

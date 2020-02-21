//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "enclave.h"
#include "pmp.h"
#include "page.h"
#include "cpu.h"
#include <string.h>
#include "atomic.h"
#include "platform.h"

#define ENCL_MAX  16
#define ENCL_TIME_SLICE 100000

struct enclave enclaves[ENCL_MAX];
#define ENCLAVE_EXISTS(eid) (eid >= 0 && eid < ENCL_MAX && enclaves[eid].state >= 0)

static spinlock_t encl_lock = SPINLOCK_INIT;

extern void save_host_regs(void);
extern void restore_host_regs(void);
extern byte dev_public_key[PUBLIC_KEY_SIZE];

/****************************
 *
 * Enclave utility functions
 * Internal use by SBI calls
 *
 ****************************/

/* Internal function containing the core of the context switching
 * code to the enclave.
 *
 * Used by resume_enclave and run_enclave.
 *
 * Expects that eid has already been valided, and it is OK to run this enclave
*/
static inline enclave_ret_code context_switch_to_enclave(uintptr_t* regs,
                                                enclave_id eid,
                                                int load_parameters){

  /* save host context */
  swap_prev_state(&enclaves[eid].threads[0], regs);
  swap_prev_mepc(&enclaves[eid].threads[0], read_csr(mepc));

  if(load_parameters){
    // passing parameters for a first run
    // $mepc: (VA) kernel entry
    write_csr(mepc, (uintptr_t) enclaves[eid].params.runtime_entry);
    // $sepc: (VA) user entry
    write_csr(sepc, (uintptr_t) enclaves[eid].params.user_entry);
    // $a1: (PA) DRAM base,
    regs[11] = (uintptr_t) enclaves[eid].pa_params.dram_base;
    // $a2: (PA) DRAM size,
    regs[12] = (uintptr_t) enclaves[eid].pa_params.dram_size;
    // $a3: (PA) kernel location,
    regs[13] = (uintptr_t) enclaves[eid].pa_params.runtime_base;
    // $a4: (PA) user location,
    regs[14] = (uintptr_t) enclaves[eid].pa_params.user_base;
    // $a5: (PA) freemem location,
    regs[15] = (uintptr_t) enclaves[eid].pa_params.free_base;
    // $a6: (VA) utm base,
    regs[16] = (uintptr_t) enclaves[eid].params.untrusted_ptr;
    // $a7: (size_t) utm size
    regs[17] = (uintptr_t) enclaves[eid].params.untrusted_size;

    // switch to the initial enclave page table
    write_csr(satp, enclaves[eid].encl_satp);
  }

  switch_vector_enclave();

  hls_t* hls = HLS();
  *hls->timecmp = getRTC() + ENCL_TIME_SLICE;

  clear_csr(mip, MIP_MTIP);
  clear_csr(mip, MIP_STIP);
  clear_csr(mip, MIP_SSIP);
  clear_csr(mip, MIP_SEIP);

  set_csr(mie, MIP_MTIP);

  uintptr_t interrupts = MIP_SSIP | MIP_SEIP;
  write_csr(mideleg, interrupts);

  // set PMP
  osm_pmp_set(PMP_NO_PERM);
  int memid;
  for(memid=0; memid < ENCLAVE_REGIONS_MAX; memid++) {
    if(enclaves[eid].regions[memid].type != REGION_INVALID) {
      pmp_set(enclaves[eid].regions[memid].pmp_rid, PMP_ALL_PERM);
    }
  }

  // Setup any platform specific defenses
  platform_switch_to_enclave(&(enclaves[eid]));
  cpu_enter_enclave_context(eid);
  swap_prev_mpp(&enclaves[eid].threads[0], regs);
  return ENCLAVE_SUCCESS;
}

static inline void context_switch_to_host(uintptr_t* encl_regs,
    enclave_id eid){

  // set PMP
  int memid;
  for(memid=0; memid < ENCLAVE_REGIONS_MAX; memid++) {
    if(enclaves[eid].regions[memid].type != REGION_INVALID) {
      pmp_set(enclaves[eid].regions[memid].pmp_rid, PMP_NO_PERM);
    }
  }
  osm_pmp_set(PMP_ALL_PERM);

  /* restore host context */
  swap_prev_state(&enclaves[eid].threads[0], encl_regs);
  swap_prev_mepc(&enclaves[eid].threads[0], read_csr(mepc));

  switch_vector_host();

  uintptr_t interrupts = MIP_SSIP | MIP_STIP | MIP_SEIP;
  write_csr(mideleg, interrupts);

  // enable timer interrupt
  clear_csr(mip, MIP_STIP);
  clear_csr(mip, MIP_MTIP);

  // Reconfigure platform specific defenses
  platform_switch_from_enclave(&(enclaves[eid]));

  cpu_exit_enclave_context();
  swap_prev_mpp(&enclaves[eid].threads[0], encl_regs);
  return;
}


// TODO: This function is externally used.
// refactoring needed
/*
 * Init all metadata as needed for keeping track of enclaves
 * Called once by the SM on startup
 */
void enclave_init_metadata(){
  enclave_id eid;
  int i=0;

  /* Assumes eids are incrementing values, which they are for now */
  for(eid=0; eid < ENCL_MAX; eid++){
    enclaves[eid].state = INVALID;

    // Clear out regions
    for(i=0; i < ENCLAVE_REGIONS_MAX; i++){
      enclaves[eid].regions[i].type = REGION_INVALID;
    }
    /* Fire all platform specific init for each enclave */
    platform_init_enclave(&(enclaves[eid]));
  }

}

static enclave_ret_code clean_enclave_memory(uintptr_t utbase, uintptr_t utsize)
{

  // This function is quite temporary. See issue #38

  // Zero out the untrusted memory region, since it may be in
  // indeterminate state.
  memset((void*)utbase, 0, utsize);

  return ENCLAVE_SUCCESS;
}

static enclave_ret_code encl_alloc_eid(enclave_id* _eid)
{
  enclave_id eid;

  spinlock_lock(&encl_lock);

  for(eid=0; eid<ENCL_MAX; eid++)
  {
    if(enclaves[eid].state == INVALID){
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

static enclave_ret_code encl_free_eid(enclave_id eid)
{
  spinlock_lock(&encl_lock);
  enclaves[eid].state = INVALID;
  spinlock_unlock(&encl_lock);
  return ENCLAVE_SUCCESS;
}

int get_enclave_region_index(enclave_id eid, enum enclave_region_type type){
  size_t i;
  for(i = 0;i < ENCLAVE_REGIONS_MAX; i++){
    if(enclaves[eid].regions[i].type == type){
      return i;
    }
  }
  // No such region for this enclave
  return -1;
}

uintptr_t get_enclave_region_size(enclave_id eid, int memid)
{
  if (0 <= memid && memid < ENCLAVE_REGIONS_MAX)
    return pmp_region_get_size(enclaves[eid].regions[memid].pmp_rid);

  return 0;
}

uintptr_t get_enclave_region_base(enclave_id eid, int memid)
{
  if (0 <= memid && memid < ENCLAVE_REGIONS_MAX)
    return pmp_region_get_addr(enclaves[eid].regions[memid].pmp_rid);

  return 0;
}

/* Ensures that dest ptr is in host, not in enclave regions
 */
static enclave_ret_code copy_word_to_host(uintptr_t* dest_ptr, uintptr_t value)
{
  int region_overlap = 0;
  spinlock_lock(&encl_lock);
  region_overlap = pmp_detect_region_overlap_atomic((uintptr_t)dest_ptr,
                                                sizeof(uintptr_t));
  if(!region_overlap)
    *dest_ptr = value;
  spinlock_unlock(&encl_lock);

  if(region_overlap)
    return ENCLAVE_REGION_OVERLAPS;
  else
    return ENCLAVE_SUCCESS;
}

// TODO: This function is externally used by sm-sbi.c.
// Change it to be internal (remove from the enclave.h and make static)
/* Internal function enforcing a copy source is from the untrusted world.
 * Does NOT do verification of dest, assumes caller knows what that is.
 * Dest should be inside the SM memory.
 */
enclave_ret_code copy_from_host(void* source, void* dest, size_t size){

  int region_overlap = 0;
  spinlock_lock(&encl_lock);
  region_overlap = pmp_detect_region_overlap_atomic((uintptr_t) source, size);
  // TODO: Validate that dest is inside the SM.
  if(!region_overlap)
    memcpy(dest, source, size);
  spinlock_unlock(&encl_lock);

  if(region_overlap)
    return ENCLAVE_REGION_OVERLAPS;
  else
    return ENCLAVE_SUCCESS;
}

static int buffer_in_enclave_region(struct enclave* enclave,
                                    void* start, size_t size){
  int legal = 0;

  int i;
  /* Check if the source is in a valid region */
  for(i = 0; i < ENCLAVE_REGIONS_MAX; i++){
    if(enclave->regions[i].type == REGION_INVALID ||
       enclave->regions[i].type == REGION_UTM)
      continue;
    uintptr_t region_start = pmp_region_get_addr(enclave->regions[i].pmp_rid);
    size_t region_size = pmp_region_get_size(enclave->regions[i].pmp_rid);
    if(start >= (void*)region_start
       && start + size <= (void*)(region_start + region_size)){
      return 1;
    }
  }
  return 0;
}

/* copies data from enclave, source must be inside EPM */
static enclave_ret_code copy_from_enclave(struct enclave* enclave,
                                          void* dest, void* source, size_t size) {

  spinlock_lock(&encl_lock);
  int legal = buffer_in_enclave_region(enclave, source, size);

  if(legal)
    memcpy(dest, source, size);
  spinlock_unlock(&encl_lock);

  if(!legal)
    return ENCLAVE_ILLEGAL_ARGUMENT;
  else
    return ENCLAVE_SUCCESS;
}

/* copies data into enclave, destination must be inside EPM */
static enclave_ret_code copy_to_enclave(struct enclave* enclave,
                                        void* dest, void* source, size_t size) {
  spinlock_lock(&encl_lock);
  int legal = buffer_in_enclave_region(enclave, dest, size);

  if(legal)
    memcpy(dest, source, size);
  spinlock_unlock(&encl_lock);

  if(!legal)
    return ENCLAVE_ILLEGAL_ARGUMENT;
  else
    return ENCLAVE_SUCCESS;
}

static int is_create_args_valid(struct keystone_sbi_create* args)
{
  uintptr_t epm_start, epm_end;

  /* printm("[create args info]: \r\n\tepm_addr: %llx\r\n\tepmsize: %llx\r\n\tutm_addr: %llx\r\n\tutmsize: %llx\r\n\truntime_addr: %llx\r\n\tuser_addr: %llx\r\n\tfree_addr: %llx\r\n", */
  /*        args->epm_region.paddr, */
  /*        args->epm_region.size, */
  /*        args->utm_region.paddr, */
  /*        args->utm_region.size, */
  /*        args->runtime_paddr, */
  /*        args->user_paddr, */
  /*        args->free_paddr); */

  // check if physical addresses are valid
  if (args->epm_region.size <= 0)
    return 0;

  // check if overflow
  if (args->epm_region.paddr >=
      args->epm_region.paddr + args->epm_region.size)
    return 0;
  if (args->utm_region.paddr >=
      args->utm_region.paddr + args->utm_region.size)
    return 0;

  epm_start = args->epm_region.paddr;
  epm_end = args->epm_region.paddr + args->epm_region.size;

  // check if physical addresses are in the range
  if (args->runtime_paddr < epm_start ||
      args->runtime_paddr >= epm_end)
    return 0;
  if (args->user_paddr < epm_start ||
      args->user_paddr >= epm_end)
    return 0;
  if (args->free_paddr < epm_start ||
      args->free_paddr > epm_end)
      // note: free_paddr == epm_end if there's no free memory
    return 0;

  // check the order of physical addresses
  if (args->runtime_paddr > args->user_paddr)
    return 0;
  if (args->user_paddr > args->free_paddr)
    return 0;

  return 1;
}

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
enclave_ret_code create_enclave(struct keystone_sbi_create create_args)
{
  /* EPM and UTM parameters */
  uintptr_t base = create_args.epm_region.paddr;
  size_t size = create_args.epm_region.size;
  uintptr_t utbase = create_args.utm_region.paddr;
  size_t utsize = create_args.utm_region.size;
  enclave_id* eidptr = create_args.eid_pptr;

  uint8_t perm = 0;
  enclave_id eid;
  enclave_ret_code ret;
  int region, shared_region;
  int i;
  int region_overlap = 0;

  /* Runtime parameters */
  if(!is_create_args_valid(&create_args))
    return ENCLAVE_ILLEGAL_ARGUMENT;

  /* set va params */
  struct runtime_va_params_t params = create_args.params;
  struct runtime_pa_params pa_params;
  pa_params.dram_base = base;
  pa_params.dram_size = size;
  pa_params.runtime_base = create_args.runtime_paddr;
  pa_params.user_base = create_args.user_paddr;
  pa_params.free_base = create_args.free_paddr;


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

  // cleanup some memory regions for sanity See issue #38
  clean_enclave_memory(utbase, utsize);


  // initialize enclave metadata
  enclaves[eid].eid = eid;

  enclaves[eid].regions[0].pmp_rid = region;
  enclaves[eid].regions[0].type = REGION_EPM;
  enclaves[eid].regions[1].pmp_rid = shared_region;
  enclaves[eid].regions[1].type = REGION_UTM;

  enclaves[eid].encl_satp = ((base >> RISCV_PGSHIFT) | SATP_MODE_CHOICE);
  enclaves[eid].n_thread = 0;
  enclaves[eid].params = params;
  enclaves[eid].pa_params = pa_params;

  /* Init enclave state (regs etc) */
  clean_state(&enclaves[eid].threads[0]);

  /* Platform create happens as the last thing before hashing/etc since
     it may modify the enclave struct */
  ret = platform_create_enclave(&enclaves[eid]);
  if(ret != ENCLAVE_SUCCESS)
    goto free_shared_region;

  /* Validate memory, prepare hash and signature for attestation */
  spinlock_lock(&encl_lock); // FIXME This should error for second enter.
  ret = validate_and_hash_enclave(&enclaves[eid]);
  /* The enclave is fresh if it has been validated and hashed but not run yet. */
  if(ret == ENCLAVE_SUCCESS)
    enclaves[eid].state = FRESH;
  spinlock_unlock(&encl_lock);

  if(ret != ENCLAVE_SUCCESS)
    goto free_platform;

  /* EIDs are unsigned int in size, copy via simple copy */
  copy_word_to_host((uintptr_t*)eidptr, (uintptr_t)eid);

  return ENCLAVE_SUCCESS;

free_platform:
  platform_destroy_enclave(&enclaves[eid]);
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
enclave_ret_code destroy_enclave(enclave_id eid)
{
  int destroyable;

  spinlock_lock(&encl_lock);
  destroyable = (ENCLAVE_EXISTS(eid)
                 && enclaves[eid].state <= STOPPED);
  /* update the enclave state first so that
   * no SM can run the enclave any longer */
  if(destroyable)
    enclaves[eid].state = DESTROYING;
  spinlock_unlock(&encl_lock);

  if(!destroyable)
    return ENCLAVE_NOT_DESTROYABLE;


  // 0. Let the platform specifics do cleanup/modifications
  platform_destroy_enclave(&enclaves[eid]);


  // 1. clear all the data in the enclave pages
  // requires no lock (single runner)
  int i;
  void* base;
  size_t size;
  region_id rid;
  for(i = 0; i < ENCLAVE_REGIONS_MAX; i++){
    if(enclaves[eid].regions[i].type == REGION_INVALID ||
       enclaves[eid].regions[i].type == REGION_UTM)
      continue;
    //1.a Clear all pages
    rid = enclaves[eid].regions[i].pmp_rid;
    base = (void*) pmp_region_get_addr(rid);
    size = (size_t) pmp_region_get_size(rid);
    memset((void*) base, 0, size);

    //1.b free pmp region
    pmp_unset_global(rid);
    pmp_region_free_atomic(rid);
  }

  // 2. free pmp region for UTM
  rid = get_enclave_region_index(eid, REGION_UTM);
  if(rid != -1)
    pmp_region_free_atomic(enclaves[eid].regions[rid].pmp_rid);

  enclaves[eid].encl_satp = 0;
  enclaves[eid].n_thread = 0;
  enclaves[eid].params = (struct runtime_va_params_t) {0};
  enclaves[eid].pa_params = (struct runtime_pa_params) {0};
  for(i=0; i < ENCLAVE_REGIONS_MAX; i++){
    enclaves[eid].regions[i].type = REGION_INVALID;
  }

  // 3. release eid
  encl_free_eid(eid);

  return ENCLAVE_SUCCESS;
}


enclave_ret_code run_enclave(uintptr_t* host_regs, enclave_id eid)
{
  int runable;

  spinlock_lock(&encl_lock);
  runable = (ENCLAVE_EXISTS(eid)
            && enclaves[eid].state == FRESH);
  if(runable) {
    enclaves[eid].state = RUNNING;
    enclaves[eid].n_thread++;
  }
  spinlock_unlock(&encl_lock);

  if(!runable) {
    return ENCLAVE_NOT_FRESH;
  }

  // Enclave is OK to run, context switch to it
  return context_switch_to_enclave(host_regs, eid, 1);
}

enclave_ret_code exit_enclave(uintptr_t* encl_regs, unsigned long retval, enclave_id eid)
{
  int exitable;

  spinlock_lock(&encl_lock);
  exitable = enclaves[eid].state == RUNNING;
  if (exitable) {
    enclaves[eid].n_thread--;
    if(enclaves[eid].n_thread == 0)
      enclaves[eid].state = STOPPED;
  }
  spinlock_unlock(&encl_lock);

  if(!exitable)
    return ENCLAVE_NOT_RUNNING;

  context_switch_to_host(encl_regs, eid);

  return ENCLAVE_SUCCESS;
}

enclave_ret_code stop_enclave(uintptr_t* encl_regs, uint64_t request, enclave_id eid)
{
  int stoppable;

  spinlock_lock(&encl_lock);
  stoppable = enclaves[eid].state == RUNNING;
  if (stoppable) {
    enclaves[eid].n_thread--;
    if(enclaves[eid].n_thread == 0)
      enclaves[eid].state = STOPPED;
  }
  spinlock_unlock(&encl_lock);

  if(!stoppable)
    return ENCLAVE_NOT_RUNNING;

  context_switch_to_host(encl_regs, eid);

  switch(request) {
  case(STOP_TIMER_INTERRUPT):
    return ENCLAVE_INTERRUPTED;
  case(STOP_EDGE_CALL_HOST):
    return ENCLAVE_EDGE_CALL_HOST;
  default:
    return ENCLAVE_UNKNOWN_ERROR;
  }
}

enclave_ret_code resume_enclave(uintptr_t* host_regs, enclave_id eid)
{
  int resumable;

  spinlock_lock(&encl_lock);
  resumable = (ENCLAVE_EXISTS(eid)
               && (enclaves[eid].state == RUNNING || enclaves[eid].state == STOPPED)
               && enclaves[eid].n_thread < MAX_ENCL_THREADS);
  if(!resumable) {
    spinlock_unlock(&encl_lock);
    return ENCLAVE_NOT_RESUMABLE;
  } else {
    enclaves[eid].n_thread++;
    enclaves[eid].state = RUNNING;
  }
  spinlock_unlock(&encl_lock);

  // Enclave is OK to resume, context switch to it
  return context_switch_to_enclave(host_regs, eid, 0);
}

enclave_ret_code attest_enclave(uintptr_t report_ptr, uintptr_t data, uintptr_t size, enclave_id eid)
{
  int attestable;
  struct report report;
  int ret;

  if (size > ATTEST_DATA_MAXLEN)
    return ENCLAVE_ILLEGAL_ARGUMENT;

  spinlock_lock(&encl_lock);
  attestable = (ENCLAVE_EXISTS(eid)
                && (enclaves[eid].state >= FRESH));
  spinlock_unlock(&encl_lock);

  if(!attestable)
    return ENCLAVE_NOT_INITIALIZED;

  /* copy data to be signed */
  ret = copy_from_enclave(&enclaves[eid],
      report.enclave.data,
      (void*)data,
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
      sizeof(struct enclave_report)
      - SIGNATURE_SIZE
      - ATTEST_DATA_MAXLEN + size);

  /* copy report to the enclave */
  ret = copy_to_enclave(&enclaves[eid],
      (void*)report_ptr,
      &report,
      sizeof(struct report));
  if (ret) {
    return ret;
  }

  return ENCLAVE_SUCCESS;
}

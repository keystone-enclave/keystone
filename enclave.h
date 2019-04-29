//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#ifndef TARGET_PLATFORM_HEADER
#error "SM requires a defined platform to build"
#endif

#include "sm.h"
#include "bits.h"
#include "vm.h"
#include "pmp.h"
#include "thread.h"
#include "crypto.h"

// Special target platform header, set by configure script
#include TARGET_PLATFORM_HEADER

#define ATTEST_DATA_MAXLEN  1024
/* TODO: does not support multithreaded enclave yet */
#define MAX_ENCL_THREADS 1

typedef enum {
  DESTROYED = -2,
  INVALID = -1,
  FRESH = 0,
  INITIALIZED,
  RUNNING,
  ALLOCATED,
} enclave_state_t;

/* Enclave stop reasons requested */
#define STOP_TIMER_INTERRUPT  0
#define STOP_EDGE_CALL_HOST   1
#define STOP_EXIT_ENCLAVE     2

/* For now, eid's are a simple unsigned int */
typedef unsigned int eid_t;

/* enclave metadata */
struct enclave_t
{
  //spinlock_t lock; //local enclave lock. we don't need this until we have multithreaded enclave
  eid_t eid; //enclave id
  int rid; //region id
  int utrid; // untrusted shared region id
  unsigned long host_satp; //supervisor satp
  unsigned long encl_satp; // enclave's page table base
  enclave_state_t state; // global state of the enclave

  /* measurement */
  byte hash[MDSIZE];
  byte sign[SIGNATURE_SIZE];

  /* parameters */
  struct runtime_va_params_t params;
  struct runtime_pa_params_t pa_params;

  /* enclave execution context */
  unsigned int n_thread;
  struct thread_state_t threads[MAX_ENCL_THREADS];

  struct platform_enclave_data_t ped;
};

/* attestation reports */
struct enclave_report_t
{
  byte hash[MDSIZE];
  uint64_t data_len;
  byte data[ATTEST_DATA_MAXLEN];
  byte signature[SIGNATURE_SIZE];
};
struct sm_report_t
{
  byte hash[MDSIZE];
  byte public_key[PUBLIC_KEY_SIZE];
  byte signature[SIGNATURE_SIZE];
};
struct report_t
{
  struct enclave_report_t enclave;
  struct sm_report_t sm;
  byte dev_public_key[PUBLIC_KEY_SIZE];
};

/*** SBI functions & external functions ***/
// callables from the host
enclave_ret_t create_enclave(struct keystone_sbi_create_t create_args);
enclave_ret_t destroy_enclave(eid_t eid);
enclave_ret_t run_enclave(uintptr_t* host_regs, eid_t eid);
enclave_ret_t resume_enclave(uintptr_t* regs, eid_t eid);
// callables from the enclave
enclave_ret_t exit_enclave(uintptr_t* regs, unsigned long retval, eid_t eid);
enclave_ret_t stop_enclave(uintptr_t* regs, uint64_t request, eid_t eid);
enclave_ret_t attest_enclave(uintptr_t report, uintptr_t data, uintptr_t size, eid_t eid);
/* attestation and virtual mapping validation */
enclave_ret_t validate_and_hash_enclave(struct enclave_t* enclave, struct keystone_sbi_create_t* cargs);
// TODO: These functions are supposed to be internal functions.
void enclave_init_metadata();
enclave_ret_t copy_from_host(void* source, void* dest, size_t size);
enclave_ret_t get_host_satp(eid_t eid, unsigned long* satp);

#endif

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
} enclave_state_kt;

/* Enclave stop reasons requested */
#define STOP_TIMER_INTERRUPT  0
#define STOP_EDGE_CALL_HOST   1
#define STOP_EXIT_ENCLAVE     2

/* For now, eid's are a simple unsigned int */
typedef unsigned int eid_kt;

/* enclave metadata */
struct enclave
{
  //spinlock_t lock; //local enclave lock. we don't need this until we have multithreaded enclave
  eid_kt eid; //enclave id
  int rid; //region id
  int utrid; // untrusted shared region id
  unsigned long host_satp; //supervisor satp
  unsigned long encl_satp; // enclave's page table base
  enclave_state_kt state; // global state of the enclave

  /* measurement */
  byte hash[MDSIZE];
  byte sign[SIGNATURE_SIZE];

  /* parameters */
  struct runtime_va_params_t params;
  struct runtime_pa_params pa_params;

  /* enclave execution context */
  unsigned int n_thread;
  struct thread_state threads[MAX_ENCL_THREADS];

  struct platform_enclave_data ped;
};

/* attestation reports */
struct enclave_report
{
  byte hash[MDSIZE];
  uint64_t data_len;
  byte data[ATTEST_DATA_MAXLEN];
  byte signature[SIGNATURE_SIZE];
};
struct sm_report
{
  byte hash[MDSIZE];
  byte public_key[PUBLIC_KEY_SIZE];
  byte signature[SIGNATURE_SIZE];
};
struct report
{
  struct enclave_report enclave;
  struct sm_report sm;
  byte dev_public_key[PUBLIC_KEY_SIZE];
};

/*** SBI functions & external functions ***/
// callables from the host
enclave_ret_kt create_enclave(struct keystone_sbi_create create_args);
enclave_ret_kt destroy_enclave(eid_kt eid);
enclave_ret_kt run_enclave(uintptr_t* host_regs, eid_kt eid);
enclave_ret_kt resume_enclave(uintptr_t* regs, eid_kt eid);
// callables from the enclave
enclave_ret_kt exit_enclave(uintptr_t* regs, unsigned long retval, eid_kt eid);
enclave_ret_kt stop_enclave(uintptr_t* regs, uint64_t request, eid_kt eid);
enclave_ret_kt attest_enclave(uintptr_t report, uintptr_t data, uintptr_t size, eid_kt eid);
/* attestation and virtual mapping validation */
enclave_ret_kt validate_and_hash_enclave(struct enclave* enclave, struct keystone_sbi_create* cargs);
// TODO: These functions are supposed to be internal functions.
void enclave_init_metadata();
enclave_ret_kt copy_from_host(void* source, void* dest, size_t size);
enclave_ret_kt get_host_satp(eid_kt eid, unsigned long* satp);

#endif

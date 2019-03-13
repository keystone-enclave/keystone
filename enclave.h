//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#ifndef TARGET_PLATFORM_HEADER
#error "SM requires a defined platform to build"
#endif

#include "pmp.h"
#include "thread.h"
#include "sm.h"
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
  struct runtime_params_t params;

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

/*** Internal utils ***/
void enclave_init_metadata();
enclave_ret_t _context_switch_to_enclave(uintptr_t* regs,
                                         eid_t eid,
                                         int load_parameters);
void _context_switch_to_host(uintptr_t* encl_regs,
                             eid_t eid);
enclave_ret_t init_enclave_memory(uintptr_t base, uintptr_t size,
                        uintptr_t utbase, uintptr_t utsize);
enclave_ret_t encl_satp_to_eid(uintptr_t satp, eid_t* eid);
enclave_ret_t host_satp_to_eid(uintptr_t satp, eid_t* eid);
enclave_ret_t encl_alloc_eid(eid_t* eid);
enclave_ret_t encl_free_eid(eid_t eid);
enclave_ret_t get_host_satp(eid_t eid, unsigned long* satp);
enclave_ret_t copy_word_to_host(uintptr_t* dest_ptr, uintptr_t value);
enclave_ret_t copy_from_host(void* source, void* dest, size_t size);
enclave_ret_t copy_from_enclave(struct enclave_t* enclave,
                      void* dest, void* source, size_t size);
enclave_ret_t copy_to_enclave(struct enclave_t* enclave,
                    void* dest, void* source, size_t size);

/*** SBI functions ***/
enclave_ret_t create_enclave(struct keystone_sbi_create_t create_args);
enclave_ret_t destroy_enclave(eid_t eid);
enclave_ret_t run_enclave(uintptr_t* host_regs, eid_t eid);
enclave_ret_t exit_enclave(uintptr_t* regs, unsigned long retval);
enclave_ret_t stop_enclave(uintptr_t* regs, uint64_t request);
enclave_ret_t resume_enclave(uintptr_t* regs, eid_t eid);
enclave_ret_t attest_enclave(uintptr_t report, uintptr_t data, uintptr_t size);

/* attestation */
enclave_ret_t hash_enclave(struct enclave_t* enclave);
#endif

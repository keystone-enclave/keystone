//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#include "pmp.h"
#include "thread.h"
#include "sm.h"
#include "crypto.h"

#define ATTEST_DATA_MAXLEN  1024
/* TODO: does not support multithreaded enclave yet */
#define MAX_ENCL_THREADS 1

typedef enum {
  DESTROYED = -2,
  INVALID = -1,
  FRESH = 0,
  INITIALIZED,
  RUNNING,
} enclave_state_t;

/* enclave metadata */
struct enclave_t
{
  //spinlock_t lock; //local enclave lock. we don't need this until we have multithreaded enclave
  unsigned int eid; //enclave id
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
enclave_ret_t _context_switch_to_enclave(uintptr_t* regs,
                                         unsigned int eid,
                                         int load_parameters);
int init_enclave_memory(uintptr_t base, uintptr_t size,
                        uintptr_t utbase, uintptr_t utsize);
int encl_satp_to_eid(uintptr_t satp, unsigned int* eid);
int host_satp_to_eid(uintptr_t satp, unsigned int* eid);
int encl_alloc_idx();
int encl_free_idx(int idx);
enclave_ret_t get_host_satp(unsigned int eid, unsigned long* satp);
int copy_word_to_host(uintptr_t* dest_ptr, uintptr_t value);
enclave_ret_t copy_from_host(void* source, void* dest, size_t size);
int copy_from_enclave(struct enclave_t* enclave,
                      void* dest, void* source, size_t size);
int copy_to_enclave(struct enclave_t* enclave,
                    void* dest, void* source, size_t size);

/*** SBI functions ***/
enclave_ret_t create_enclave(struct keystone_sbi_create_t create_args);
enclave_ret_t destroy_enclave(unsigned int eid);
enclave_ret_t run_enclave(uintptr_t* host_regs, unsigned int eid);
enclave_ret_t exit_enclave(uintptr_t* regs, unsigned long retval);
enclave_ret_t stop_enclave(uintptr_t* regs, uint64_t request);
enclave_ret_t resume_enclave(uintptr_t* regs, unsigned int eid);
enclave_ret_t attest_enclave(uintptr_t report, uintptr_t data, uintptr_t size);

/* attestation */
enclave_ret_t hash_enclave(struct enclave_t* enclave);
#endif

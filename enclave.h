#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#include "pmp.h"
#include "thread.h"
#include "keystone-sbi-arg.h"
#include "sm.h"
#include "crypto.h"
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
  unsigned int eid; //enclave id
  int rid; //region id
  int utrid; // untrusted shared region id
  unsigned long host_satp; //supervisor satp
  unsigned long encl_satp; // enclave's page table base
  enclave_state_t state; // global state of the enclave

  /* measurement */
  byte hash[MDSIZE];
  byte sign[SIGNATURE_SIZE];

  /* entry points */
  uintptr_t enclave_entry;
  uintptr_t runtime_entry;

  /* enclave execution context */
  unsigned int n_thread;
  struct thread_state_t threads[MAX_ENCL_THREADS];
};

/* attestation report */
struct report_t
{
  byte sm_hash[MDSIZE];
  byte sm_public_key[PUBLIC_KEY_SIZE];
  byte sm_signature[SIGNATURE_SIZE];
  byte enclave_hash[MDSIZE];
  byte enclave_signature[SIGNATURE_SIZE];
};

int copy_region_from_host(void* source, void* dest, size_t size);

unsigned long get_host_satp(unsigned int eid);
enclave_ret_t create_enclave(struct keystone_sbi_create_t create_args);
enclave_ret_t destroy_enclave(unsigned int eid);

enclave_ret_t run_enclave(uintptr_t* host_regs, unsigned int eid);
enclave_ret_t exit_enclave(uintptr_t* regs, unsigned long retval);
enclave_ret_t stop_enclave(uintptr_t* regs, uint64_t request);
enclave_ret_t resume_enclave(uintptr_t* regs, unsigned int eid);

/* attestation */
enclave_ret_t hash_enclave(struct enclave_t* enclave);
enclave_ret_t sign_enclave(struct enclave_t* enclave);
#endif

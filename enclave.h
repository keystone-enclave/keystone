#ifndef _ENCLAVE_H_
#define _ENCLAVE_H_

#include "pmp.h"

/* TODO: does not support multithreaded enclave yet */
#define MAX_ENCL_THREADS 1

typedef enum {
  DESTROYED = -2,
  INVALID = -1,
  FRESH = 0,
  INITIALIZED,
  RUNNING,
} enclave_state_t;

#if __riscv_xlen == 32
typedef uint32_t reg;
#else
typedef uint64_t reg;
#endif

struct ctx_t
{
  reg ra;
  reg sp;
  reg gp;
  reg tp;
  reg t0;
  reg t1;
  reg t2;
  reg s0;
  reg s1;
  reg a0;
  reg a1;
  reg a2;
  reg a3;
  reg a4;
  reg a5;
  reg a6;
  reg a7;
  reg s2;
  reg s3;
  reg s4;
  reg s5;
  reg s6;
  reg s7;
  reg s8;
  reg s9;
  reg s10;
  reg s11;
  reg t3;
  reg t4;
  reg t5;
  reg t6;
};

struct enclave_thread_t
{
  uintptr_t entry;
  unsigned long* retptr; // pointer to the host return value
};

struct enclave_t
{
  int eid; //enclave id
  int rid; //region id
  unsigned long host_satp; //supervisor satp
  unsigned long encl_satp; // enclave's page table base
  enclave_state_t state; // global state of the enclave
  unsigned int n_thread;

  /* execution context */
  unsigned long host_mepc[MAX_HARTS]; // supervisor return pc
  unsigned long host_stvec[MAX_HARTS]; // supervisor stvec
  
  /* enclave execution context */
  struct enclave_thread_t threads[MAX_ENCL_THREADS];
};

unsigned long get_host_satp(int eid);
uintptr_t create_enclave(uintptr_t base, uintptr_t size);
uintptr_t destroy_enclave(int eid);
uintptr_t run_enclave(int eid, uintptr_t entry, uintptr_t retptr);
uintptr_t exit_enclave(unsigned long retval);
uintptr_t stop_enclave(uint64_t request);
uintptr_t resume_enclave(int eid);

#endif

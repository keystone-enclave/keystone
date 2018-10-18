#ifndef __THREAD_H__
#define __THREAD_H__

#include <stdint.h>
struct ctx_t
{
  uintptr_t slot;
  uintptr_t ra;
  uintptr_t sp;
  uintptr_t gp;
  uintptr_t tp;
  uintptr_t t0;
  uintptr_t t1;
  uintptr_t t2;
  uintptr_t s0;
  uintptr_t s1;
  uintptr_t a0;
  uintptr_t a1;
  uintptr_t a2;
  uintptr_t a3;
  uintptr_t a4;
  uintptr_t a5;
  uintptr_t a6;
  uintptr_t a7;
  uintptr_t s2;
  uintptr_t s3;
  uintptr_t s4;
  uintptr_t s5;
  uintptr_t s6;
  uintptr_t s7;
  uintptr_t s8;
  uintptr_t s9;
  uintptr_t s10;
  uintptr_t s11;
  uintptr_t t3;
  uintptr_t t4;
  uintptr_t t5;
  uintptr_t t6;
};

/* enclave thread state */
struct thread_state_t
{
  uintptr_t prev_mepc;
  struct ctx_t prev_state;
  unsigned long* retptr; // pointer to the host return value
};

/* swap previous and current thread states */
void swap_prev_state(struct thread_state_t* state, uintptr_t* regs);
void swap_prev_mepc(struct thread_state_t* state, uintptr_t mepc);
void set_retptr(struct thread_state_t* state, unsigned long* retptr);
#endif /* thread */

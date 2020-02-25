//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "thread.h"
#include "mtrap.h"


void switch_vector_enclave(){
  extern void trap_vector_enclave();
  write_csr(mtvec, &trap_vector_enclave);
}

void switch_vector_host(){
  extern void trap_vector();
  write_csr(mtvec, &trap_vector);
}

uint64_t getRTC(){
	return *mtime;
}

void swap_prev_mpp(struct thread_state* thread, uintptr_t* regs){
  //Time interrupts can occur in either user mode or supervisor mode

  int curr_mstatus = read_csr(mstatus);
  int old_mpp = thread->prev_mpp;
  if(old_mpp < 0){
   //Old MPP bit isn't initialized!
   old_mpp = curr_mstatus & 0x800;
  }
  thread->prev_mpp = curr_mstatus & 0x800;
  int new_mstatus = (curr_mstatus & ~0x800) | old_mpp;
  write_csr(mstatus, new_mstatus);
}

/* Swaps the entire s-mode visible state, general registers and then csrs */
void swap_prev_state(struct thread_state* thread, uintptr_t* regs, int return_on_resume)
{
  int i;

  uintptr_t* prev = (uintptr_t*) &thread->prev_state;
  for(i=0; i<32; i++)
  {
    /* swap state */
    uintptr_t tmp = prev[i];
    prev[i] = regs[i];
    regs[i] = tmp;
  }

  prev[0] = !return_on_resume;

  swap_prev_smode_csrs(thread);

  return;
}

/* Swaps all s-mode csrs defined in 1.10 standard */
/* TODO: Right now we are only handling the ones that our test
   platforms support. Realistically we should have these behind
   defines for extensions (ex: N extension)*/
void swap_prev_smode_csrs(struct thread_state*
thread){

  uintptr_t tmp;

#define LOCAL_SWAP_CSR(csrname) \
  tmp = thread->prev_csrs.csrname;                 \
  thread->prev_csrs.csrname = read_csr(csrname);   \
  write_csr(csrname, tmp);

  LOCAL_SWAP_CSR(sstatus);
  // These only exist with N extension.
  //LOCAL_SWAP_CSR(sedeleg);
  //LOCAL_SWAP_CSR(sideleg);
  LOCAL_SWAP_CSR(sie);
  LOCAL_SWAP_CSR(stvec);
  LOCAL_SWAP_CSR(scounteren);
  LOCAL_SWAP_CSR(sscratch);
  LOCAL_SWAP_CSR(sepc);
  LOCAL_SWAP_CSR(scause);
  LOCAL_SWAP_CSR(sbadaddr);
  LOCAL_SWAP_CSR(sip);
  LOCAL_SWAP_CSR(satp);

#undef LOCAL_SWAP_CSR
}

void swap_prev_mepc(struct thread_state* thread, uintptr_t current_mepc)
{
  uintptr_t tmp = thread->prev_mepc;
  thread->prev_mepc = current_mepc;
  write_csr(mepc, tmp);
}


void clean_state(struct thread_state* state){
  int i;
  uintptr_t* prev = (uintptr_t*) &state->prev_state;
  for(i=1; i<32; i++)
  {
    prev[i] = 0;
  }

  state->prev_mpp = -1; // 0x800;
  clean_smode_csrs(state);
}

void clean_smode_csrs(struct thread_state* state){

  state->prev_csrs.sstatus = 0;

  // We can't read these or set these from M-mode?
  state->prev_csrs.sedeleg = 0;
  state->prev_csrs.sideleg = 0;

  state->prev_csrs.sie = 0;
  state->prev_csrs.stvec = 0;
  // For now we take whatever the OS was doing
  state->prev_csrs.scounteren = read_csr(scounteren);
  state->prev_csrs.sscratch = 0;
  state->prev_csrs.sepc = 0;
  state->prev_csrs.scause = 0;
  state->prev_csrs.sbadaddr = 0;
  state->prev_csrs.sip = 0;
  state->prev_csrs.satp = 0;

}

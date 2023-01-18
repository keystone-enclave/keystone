//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <sbi/riscv_asm.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_string.h>
#include "thread.h"

#define MSTATUS_MASK ((uintptr_t) (MSTATUS_SIE | MSTATUS_SPIE | MSTATUS_SPP | \
                        MSTATUS_MPP | MSTATUS_FS | MSTATUS_SUM | \
                        MSTATUS_MXR))

void switch_vector_enclave(void){
  csr_write(mtvec, &trap_vector_enclave);
}

void switch_vector_host(void){
  csr_write(mtvec, &_trap_handler);
}

void stash_prev_state(struct thread_state *state, struct sbi_trap_regs *regs, int return_on_resume) {
  uintptr_t *target = (uintptr_t *) &state->prev_state;

  sbi_memcpy(&state->prev_state, regs, 32 * sizeof(uintptr_t));
  target[0] = !return_on_resume;
  stash_prev_smode_csrs(&state->prev_csrs);
}

void stash_prev_mepc(struct thread_state *state, struct sbi_trap_regs *regs) {
  state->prev_mepc = regs->mepc;
}

void stash_prev_mstatus(struct thread_state *state, struct sbi_trap_regs *regs) {
  state->prev_mstatus = (regs->mstatus & ~MSTATUS_MASK) | (regs->mstatus & MSTATUS_MASK);
}

void stash_prev_smode_csrs(struct csrs *csrs) {
#define LOCAL_STASH_CSR(csrname) \
  csrs->csrname = csr_read(csrname)

  LOCAL_STASH_CSR(sstatus);
  // These only exist with N extension.
  //LOCAL_STASH_CSR(sedeleg);
  //LOCAL_STASH_CSR(sideleg);
  LOCAL_STASH_CSR(sie);
  LOCAL_STASH_CSR(stvec);
  LOCAL_STASH_CSR(scounteren);
  LOCAL_STASH_CSR(sscratch);
  LOCAL_STASH_CSR(sepc);
  LOCAL_STASH_CSR(scause);
  LOCAL_STASH_CSR(sbadaddr);
  LOCAL_STASH_CSR(sip);
  LOCAL_STASH_CSR(satp);
#undef LOCAL_STASH_CSR
}

void pop_prev_state(struct thread_state *state, struct sbi_trap_regs *regs) {
  sbi_memcpy(regs, &state->prev_state, 32 * sizeof(uintptr_t));
  pop_prev_smode_csrs(&state->prev_csrs);
}

void pop_prev_mepc(struct thread_state *state, struct sbi_trap_regs *regs) {
  regs->mepc = state->prev_mepc;
}

void pop_prev_mstatus(struct thread_state *state, struct sbi_trap_regs *regs) {
  regs->mstatus = (state->prev_mstatus & ~MSTATUS_MASK) | state->prev_mstatus;
}

void pop_prev_smode_csrs(struct csrs *csrs) {
#define LOCAL_POP_CSR(csrname) \
    csr_write(csrname, csrs->csrname)

  LOCAL_POP_CSR(sstatus);
  // These only exist with N extension.
  //LOCAL_POP_CSR(sedeleg);
  //LOCAL_POP_CSR(sideleg);
  LOCAL_POP_CSR(sie);
  LOCAL_POP_CSR(stvec);
  LOCAL_POP_CSR(scounteren);
  LOCAL_POP_CSR(sscratch);
  LOCAL_POP_CSR(sepc);
  LOCAL_POP_CSR(scause);
  LOCAL_POP_CSR(sbadaddr);
  LOCAL_POP_CSR(sip);
  LOCAL_POP_CSR(satp);
#undef LOCAL_POP_CSR
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
  state->prev_csrs.scounteren = csr_read(scounteren);
  state->prev_csrs.sscratch = 0;
  state->prev_csrs.sepc = 0;
  state->prev_csrs.scause = 0;
  state->prev_csrs.sbadaddr = 0;
  state->prev_csrs.sip = 0;
  state->prev_csrs.satp = 0;

}

//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "thread.h"
#include "mtrap.h"

void swap_prev_state(struct thread_state_t* thread, uintptr_t* regs)
{
  int i;

  uintptr_t* prev = (uintptr_t*) &thread->prev_state;
  for(i=1; i<32; i++)
  {
    /* swap state */
    uintptr_t tmp = prev[i];
    prev[i] = regs[i];
    regs[i] = tmp;
  }
  return;
}

void swap_prev_mepc(struct thread_state_t* thread, uintptr_t current_mepc)
{
  uintptr_t tmp = thread->prev_mepc;
  thread->prev_mepc = current_mepc;
  write_csr(mepc, tmp);
}

void swap_prev_stvec(struct thread_state_t* thread, uintptr_t current_stvec)
{
  uintptr_t tmp = thread->prev_stvec;
  thread->prev_stvec = current_stvec;
  write_csr(stvec, tmp);
}

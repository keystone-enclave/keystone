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

/* FIXME: this is illegal in security perspective, because host can give any retptr */
void write_to_retptr(struct thread_state_t* thread, unsigned long retval)
{
  *(thread->retptr) = retval;
}
void set_retptr(struct thread_state_t* thread, unsigned long* retptr)
{
  thread->retptr = retptr;
}

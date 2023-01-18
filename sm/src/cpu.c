//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "cpu.h"
#include "sm_assert.h"
#include <sbi/riscv_asm.h>

#ifndef TARGET_PLATFORM_HEADER
#error "SM requires a defined platform to build"
#endif

// Special target platform header, set by configure script
#include TARGET_PLATFORM_HEADER

static struct cpu_state cpus[MAX_HARTS] = {0,};

int cpu_is_enclave_context(void)
{
  return cpus[csr_read(mhartid)].is_enclave;
}

enclave_id cpu_get_enclave_id(void)
{
  return cpus[csr_read(mhartid)].eid;
}

void cpu_enter_enclave_context(enclave_id eid, struct sbi_trap_regs *host_state)
{
  u64 hartid = csr_read(mhartid);
  sm_assert(!cpus[hartid].is_enclave);

  cpus[hartid].is_enclave = 1;
  cpus[hartid].eid = eid;

  stash_prev_state(&cpus[hartid].host_state, host_state, 1);
  stash_prev_mepc(&cpus[hartid].host_state, host_state);
  stash_prev_mstatus(&cpus[hartid].host_state, host_state);
}

void cpu_exit_enclave_context(struct sbi_trap_regs *host_state)
{
  u64 hartid = csr_read(mhartid);
  sm_assert(cpus[hartid].is_enclave);
  cpus[csr_read(mhartid)].is_enclave = 0;

  pop_prev_state(&cpus[hartid].host_state, host_state);
  pop_prev_mepc(&cpus[hartid].host_state, host_state);
  pop_prev_mstatus(&cpus[hartid].host_state, host_state);
}

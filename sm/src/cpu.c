//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "cpu.h"
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

int cpu_get_enclave_id(void)
{
  return cpus[csr_read(mhartid)].eid;
}

void cpu_enter_enclave_context(enclave_id eid)
{
  cpus[csr_read(mhartid)].is_enclave = 1;
  cpus[csr_read(mhartid)].eid = eid;
}

void cpu_exit_enclave_context(void)
{
  cpus[csr_read(mhartid)].is_enclave = 0;
}

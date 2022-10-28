//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "cpu.h"
#include <sbi/riscv_asm.h>

static struct cpu_state cpus[MAX_HARTS] = {0,};

int cpu_is_enclave_context()
{
  return cpus[csr_read(mhartid)].is_enclave;
}

int cpu_get_enclave_id()
{
  return cpus[csr_read(mhartid)].eid;
}

void cpu_enter_enclave_context(enclave_id eid)
{
  cpus[csr_read(mhartid)].is_enclave = 1;
  cpus[csr_read(mhartid)].eid = eid;
}

void cpu_exit_enclave_context()
{
  cpus[csr_read(mhartid)].is_enclave = 0;
}

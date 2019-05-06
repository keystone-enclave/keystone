//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "cpu.h"

static struct cpu_state cpus[MAX_HARTS];

int cpu_is_enclave_context()
{
  return cpus[read_csr(mhartid)].is_enclave != 0;
}

int cpu_get_enclave_id()
{
  return cpus[read_csr(mhartid)].eid;
}

void cpu_enter_enclave_context(eid_kt eid)
{
  cpus[read_csr(mhartid)].is_enclave = 1;
  cpus[read_csr(mhartid)].eid = eid;
}

void cpu_exit_enclave_context()
{
  cpus[read_csr(mhartid)].is_enclave = 0;
}

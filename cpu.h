//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __CPU_H__
#define __CPU_H__

#include "sm.h"
#include "enclave.h"

/* hart state for regulating SBI */
struct cpu_state_t
{
  int is_enclave;
  eid_t eid;
};

/* external functions */
int cpu_is_enclave_context();
int cpu_get_enclave_id();
void cpu_enter_enclave_context(eid_t eid);
void cpu_exit_enclave_context();

#endif

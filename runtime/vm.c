//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "regs.h"
#include "vm.h"
#include "printf.h"

void rt_page_fault(struct encl_ctx_t* ctx)
{
  unsigned long addr, cause;

  addr = ctx->sbadaddr;
  cause = ctx->scause;

  printf("[runtime] page fault at 0x%lx (scause: 0x%lx)\n", addr, cause);

  sbi_exit_enclave(-1);
  return;
}

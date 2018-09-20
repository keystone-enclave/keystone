#include "regs.h"
#include "mm.h"

extern struct enclave_mm mm;

void rt_page_fault(struct regs* ctx)
{
  unsigned long addr, cause;

  addr = ctx->sbadaddr;
  cause = ctx->scause;
  
  addr = cause;
  cause = addr;

  return;
}

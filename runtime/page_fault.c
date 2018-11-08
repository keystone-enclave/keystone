#include "regs.h"
#include "mm.h"
#include "printf.h"
extern struct enclave_mm mm;

void rt_page_fault(struct encl_ctx_t* ctx)
{
  unsigned long addr, cause;


  addr = ctx->sbadaddr;
  cause = ctx->scause;
  
  printf("[runtime] page fault at 0x%lx (scause: 0x%lx)\n", addr, cause);
  SBI_CALL_1(SBI_SM_EXIT_ENCLAVE,-1);
  return;
}

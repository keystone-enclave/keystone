#include "regs.h"

 void rt_page_fault(struct regs* ctx)
{
  ctx->a0 = ctx->a1 + ctx->a2;
  return;
}

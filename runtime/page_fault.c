#include "regs.h"

 void rt_page_fault(struct pt_regs* regs)
{
  regs->a0 = regs->a1 + regs->a2;
  return;
}

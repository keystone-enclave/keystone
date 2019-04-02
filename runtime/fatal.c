#include "regs.h"
#include "printf.h"

#include "fatal.h"

#define FATAL_DEBUG 1

void not_implemented_fatal(struct encl_ctx_t* ctx){
  unsigned long addr, cause, pc;

  if(FATAL_DEBUG){

    pc = ctx->regs.sepc;
    addr = ctx->sbadaddr;
    cause = ctx->scause;

    printf("[runtime] non-handlable interrupt/exception at 0x%lx on 0x%lx (scause: 0x%lx)\r\n", pc, addr, cause);

    // Bail to m-mode
    asm volatile ("csrr a0, scause\r\nli a7, 1111\r\n ecall");

    return;
  }

}

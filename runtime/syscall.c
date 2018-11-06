#include "syscall.h"

extern void exit_enclave(uintptr_t arg0);

void handle_syscall(struct encl_ctx_t* ctx)
{
  
  uintptr_t n = ctx->regs.a7;
  uintptr_t arg0 = ctx->regs.a0;
  //uintptr_t arg1 = ctx->regs.a1;
  //uintptr_t arg2 = ctx->regs.a2;
  //uintptr_t arg3 = ctx->regs.a3;

  uintptr_t ret = 0;

  ctx->regs.sepc += 4;

  //printf("[runtime] syscall: %ld\n", n);
  switch (n) {
    case(RUNTIME_SYSCALL_EXIT):
      SBI_CALL_1(SBI_SM_EXIT_ENCLAVE, arg0);
      break;
    case(RUNTIME_SYSCALL_OCALL):
      ret = SBI_CALL_1(SBI_SM_STOP_ENCLAVE, arg0);
      break;
    /* FIXME: just fixed mapping now */
    case(RUNTIME_SYSCALL_UNTRUSTED_MMAP):
      ret = 0x0dead000;
      break;
    case(RUNTIME_SYSCALL_UNKNOWN):
    default:
      printf("[runtime] syscall %ld not implemented\n", (unsigned long) n);
      ret = -1;
      break;
  }

  /* store the result in the stack */
  ctx->regs.a0 = ret;
  return;
}

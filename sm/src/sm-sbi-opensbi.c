#include <sbi/sbi_trap.h>
#include <sbi/sbi_error.h>
#include <sbi/sbi_tlb.h>
#include <sbi/sbi_ipi.h>
#include <sbi/sbi_string.h>
#include <sbi/riscv_locks.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_scratch.h>
#include <sbi/riscv_asm.h>
#include <sbi/sbi_ecall.h>
#include "sm-sbi-opensbi.h"
#include "pmp.h"
#include "sm-sbi.h"
#include "sm.h"
#include "cpu.h"

static int sbi_ecall_keystone_enclave_handler(unsigned long extid, unsigned long funcid,
                     const struct sbi_trap_regs *regs,
                     unsigned long *out_val,
                     struct sbi_trap_info *out_trap)
{
  uintptr_t retval;

  if (funcid <= FID_RANGE_DEPRECATED) { return SBI_ERR_SM_DEPRECATED; }
  else if (funcid <= FID_RANGE_HOST)
  {
    if (cpu_is_enclave_context())
      return SBI_ERR_SM_ENCLAVE_SBI_PROHIBITED;
  }
  else if (funcid <= FID_RANGE_ENCLAVE)
  {
    if (!cpu_is_enclave_context())
      return SBI_ERR_SM_ENCLAVE_SBI_PROHIBITED;
  }

  switch (funcid) {
    case SBI_SM_CREATE_ENCLAVE:
      retval = sbi_sm_create_enclave(out_val, regs->a0);
      break;
    case SBI_SM_DESTROY_ENCLAVE:
      retval = sbi_sm_destroy_enclave(regs->a0);
      break;
    case SBI_SM_RUN_ENCLAVE:
      retval = sbi_sm_run_enclave((struct sbi_trap_regs*) regs, regs->a0);
      __builtin_unreachable();
      break;
    case SBI_SM_RESUME_ENCLAVE:
      retval = sbi_sm_resume_enclave((struct sbi_trap_regs*) regs, regs->a0);
      __builtin_unreachable();
      break;
    case SBI_SM_RANDOM:
      *out_val = sbi_sm_random();
      retval = 0;
      break;
    case SBI_SM_ATTEST_ENCLAVE:
      retval = sbi_sm_attest_enclave(regs->a0, regs->a1, regs->a2);
      break;
    case SBI_SM_GET_SEALING_KEY:
      retval = sbi_sm_get_sealing_key(regs->a0, regs->a1, regs->a2);
      break;
    case SBI_SM_CLAIM_MMIO:
      retval = sbi_sm_claim_mmio(regs->a0);
      break;
    case SBI_SM_RELEASE_MMIO:
      retval = sbi_sm_release_mmio(regs->a0);
      break;
    case SBI_SM_STOP_ENCLAVE:
      retval = sbi_sm_stop_enclave((struct sbi_trap_regs*) regs, regs->a0);
      __builtin_unreachable();
      break;
    case SBI_SM_EXIT_ENCLAVE:
      retval = sbi_sm_exit_enclave((struct sbi_trap_regs*) regs, regs->a0);
      __builtin_unreachable();
      break;
    case SBI_SM_CALL_PLUGIN:
      retval = sbi_sm_call_plugin(regs->a0, regs->a1, regs->a2, regs->a3);
      break;
    default:
      retval = SBI_ERR_SM_NOT_IMPLEMENTED;
      break;
  }

  return retval;

}

struct sbi_ecall_extension ecall_keystone_enclave = {
  .extid_start = SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE,
  .extid_end = SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE,
  .handle = sbi_ecall_keystone_enclave_handler,
};

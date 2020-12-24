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
#include "sm_sbi_opensbi.h"
#include "pmp.h"
#include "sm-sbi.h"
#include "sm.h"

static int sbi_ecall_keystone_enclave_handler(unsigned long extid, unsigned long funcid,
                     struct sbi_trap_regs *regs,
                     unsigned long *args, unsigned long *out_val,
                     struct sbi_trap_info *out_trap)
{
  uintptr_t retval;
  switch(funcid){
  case SBI_SM_CREATE_ENCLAVE:
    retval = mcall_sm_create_enclave(args[0]);
    break;
  case SBI_SM_DESTROY_ENCLAVE:
    retval = mcall_sm_destroy_enclave(args[0]);
    break;
  case SBI_SM_RUN_ENCLAVE:
    retval = mcall_sm_run_enclave(regs, args[0]);
    break;
  case SBI_SM_EXIT_ENCLAVE:
    retval = mcall_sm_exit_enclave(regs, args[0]);
    break;
  case SBI_SM_STOP_ENCLAVE:
    retval = mcall_sm_stop_enclave(regs, args[0]);
    break;
  case SBI_SM_RESUME_ENCLAVE:
    retval = mcall_sm_resume_enclave(regs, args[0]);
    break;
  case SBI_SM_ATTEST_ENCLAVE:
    retval = mcall_sm_attest_enclave(args[0], args[1], args[2]);
    break;
  case SBI_SM_RANDOM:
    retval = mcall_sm_random();
    break;
  case SBI_SM_GET_SEALING_KEY:
    retval = mcall_sm_get_sealing_key(args[0], args[1], args[2]);
    break;
  case SBI_SM_CALL_PLUGIN:
    retval = mcall_sm_call_plugin(args[0], args[1], args[2], args[3]);
    break;
  default:
    retval = ENCLAVE_NOT_IMPLEMENTED;
    break;

  }

  *out_val = regs->a1;

  return retval;

}

struct sbi_ecall_extension ecall_keystone_enclave = {
  .extid_start = SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE,
  .extid_end = SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE,
  .handle = sbi_ecall_keystone_enclave_handler,
};

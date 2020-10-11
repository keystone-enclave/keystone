#include <sbi/sbi_trap.h>
#include <sbi/sbi_error.h>
#include <sbi/sbi_tlb.h>
#include <sbi/sbi_ipi.h>
#include <sbi/sbi_string.h>
#include <sbi/riscv_locks.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_scratch.h>
#include <sbi/riscv_asm.h>
#include "sm_sbi_opensbi.h"
#include "pmp.h"
#include "sm-sbi.h"
#include "sm.h"

int sbi_sm_interface(struct sbi_scratch *scratch, unsigned long extension_id,
                     struct sbi_trap_regs  *regs,
                     unsigned long *out_val,
                     struct sbi_trap_info *out_trap){
  /* Note: regs is the sbi_trap.h format, which matches (... enough)
     the thread.h format, which means we can 'just use it' for now.
     This really needs to be handled far more cleanly. */

  uintptr_t funcid = regs->a6; /* This was a7 in the old interface */
  uintptr_t arg0 = regs->a0, arg1 = regs->a1, arg2 = regs->a2, arg3 = regs->a3;
  uintptr_t retval;

  switch(funcid){
  case SBI_SM_CREATE_ENCLAVE:
    retval = mcall_sm_create_enclave(arg0);
    break;
  case SBI_SM_DESTROY_ENCLAVE:
    retval = mcall_sm_destroy_enclave(arg0);
    break;
  case SBI_SM_RUN_ENCLAVE:
    retval = mcall_sm_run_enclave((uintptr_t*)regs, arg0);
    break;
  case SBI_SM_EXIT_ENCLAVE:
    retval = mcall_sm_exit_enclave((uintptr_t*)regs, arg0);
    break;
  case SBI_SM_STOP_ENCLAVE:
    retval = mcall_sm_stop_enclave((uintptr_t*)regs, arg0);
    break;
  case SBI_SM_RESUME_ENCLAVE:
    retval = mcall_sm_resume_enclave((uintptr_t*)regs, arg0);
    break;
  case SBI_SM_ATTEST_ENCLAVE:
    retval = mcall_sm_attest_enclave(arg0, arg1, arg2);
    break;
  case SBI_SM_RANDOM:
    retval = mcall_sm_random();
    break;
  case SBI_SM_CALL_PLUGIN:
    retval = mcall_sm_call_plugin(arg0, arg1, arg2, arg3);
    break;
  default:
    retval = ENCLAVE_NOT_IMPLEMENTED;
    break;

  }

  *out_val = retval;

  if(retval != ENCLAVE_SUCCESS){
    retval = SBI_ENOTSUPP;
  }
  else{
    retval = SBI_OK;
  }

  return retval;

}

void sm_ipi_process(){
  sbi_printf("ipi got %lx\r\n",csr_read(mhartid));
  handle_pmp_ipi();
}

int sm_sbi_send_ipi(uintptr_t recipient_mask){
  struct sbi_scratch *scratch =  sbi_scratch_thishart_ptr();
  struct sbi_trap_info uptrap;
  uptrap.epc = 0;
  uptrap.cause = 0;
  uptrap.tval = 0;
  struct sbi_tlb_info tlb_flush_info;
  tlb_flush_info.start = 0;
  tlb_flush_info.size = 0;
  sbi_printf("senidng ipi %lx\r\n", recipient_mask);
  return sbi_ipi_send_many(scratch, &uptrap, &recipient_mask,
                              SBI_SM_EVENT, &tlb_flush_info);


}

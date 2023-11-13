//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _KEYSTONE_SBI_H_
#define _KEYSTONE_SBI_H_

#include <sbi/sbi_types.h>
#include <sbi/sbi_trap.h>

unsigned long
sbi_sm_create_enclave(unsigned long *out_val, uintptr_t create_args);

unsigned long
sbi_sm_destroy_enclave(unsigned long eid);

unsigned long
sbi_sm_run_enclave(struct sbi_trap_regs *regs, unsigned long eid);

unsigned long
sbi_sm_exit_enclave(struct sbi_trap_regs *regs, unsigned long retval);

unsigned long
sbi_sm_stop_enclave(struct sbi_trap_regs *regs, unsigned long request);

unsigned long
sbi_sm_resume_enclave(struct sbi_trap_regs *regs, unsigned long eid);

unsigned long
sbi_sm_attest_enclave(uintptr_t report, uintptr_t data, uintptr_t size);

unsigned long
sbi_sm_get_sealing_key(uintptr_t seal_key, uintptr_t key_ident, size_t key_ident_size);

unsigned long
sbi_sm_claim_mmio(uintptr_t dev_string);

unsigned long
sbi_sm_release_mmio(uintptr_t dev_string);

unsigned long
sbi_sm_call_enclave(struct sbi_trap_regs *regs, unsigned long eid, int type);

unsigned long
sbi_sm_ret_enclave(struct sbi_trap_regs *regs);

unsigned long
sbi_sm_register_handler(uintptr_t handler);

unsigned long
sbi_sm_share_region(uintptr_t addr, size_t size, unsigned int with);

unsigned long
sbi_sm_unshare_region(uintptr_t addr, unsigned int with);

unsigned long
sbi_sm_random(void);

unsigned long
sbi_sm_call_plugin(uintptr_t plugin_id, uintptr_t call_id, uintptr_t arg0, uintptr_t arg1);

unsigned long 
sbi_sm_get_misc_params(uintptr_t out);

#endif

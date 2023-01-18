//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __SBI_H_
#define __SBI_H_

#include <stdint.h>
#include <stddef.h>

#include "sm_call.h"

void
sbi_putchar(char c);
void
sbi_set_timer(uint64_t stime_value);
uintptr_t
sbi_stop_enclave(uint64_t request);
void
sbi_exit_enclave(uint64_t retval);
uintptr_t
sbi_random();
uintptr_t
sbi_query_multimem(size_t *size);
uintptr_t
sbi_query_multimem_addr(uintptr_t *addr);
uintptr_t
sbi_attest_enclave(void* report, void* buf, uintptr_t len);
uintptr_t
sbi_get_sealing_key(uintptr_t key_struct, uintptr_t key_ident, uintptr_t len);
uintptr_t
sbi_claim_mmio(uintptr_t dev_string);
uintptr_t
sbi_release_mmio(uintptr_t dev_string);

#ifdef USE_CALLEE
uintptr_t
sbi_call_enclave(int eid, int type, uintptr_t arg0, uintptr_t arg1, uintptr_t arg2, uintptr_t arg3);
void
sbi_ret_enclave(uintptr_t ret) __attribute__((noreturn));
uintptr_t
sbi_register_handler(uintptr_t handler);
#endif

#endif

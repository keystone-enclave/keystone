//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef __SBI_H_
#define __SBI_H_

#include <stdint.h>

#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2

#define SBI_SM_CREATE_ENCLAVE    101
#define SBI_SM_DESTROY_ENCLAVE   102
#define SBI_SM_ATTEST_ENCLAVE    103
#define SBI_SM_GET_SEALING_KEY   104
#define SBI_SM_RUN_ENCLAVE       105
#define SBI_SM_STOP_ENCLAVE      106
#define SBI_SM_RESUME_ENCLAVE    107
#define SBI_SM_RANDOM            108
#define SBI_SM_EXIT_ENCLAVE     1101
#define SBI_SM_CALL_PLUGIN      1000
#define SBI_SM_NOT_IMPLEMENTED  1111

/* Plugin IDs and Call IDs */
#define SM_MULTIMEM_PLUGIN_ID   0x01
#define SM_MULTIMEM_CALL_GET_SIZE 0x01
#define SM_MULTIMEM_CALL_GET_ADDR 0x02

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
sbi_query_multimem();
uintptr_t
sbi_query_multimem_addr();
uintptr_t
sbi_attest_enclave(void* report, void* buf, uintptr_t len);
uintptr_t
sbi_get_sealing_key(uintptr_t key_struct, uintptr_t key_ident, uintptr_t len);

#endif

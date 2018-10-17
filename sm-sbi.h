#ifndef _KEYSTONE_SBI_H_
#define _KEYSTONE_SBI_H_

#include <stdint.h>
#include <stddef.h>
uintptr_t mcall_sm_create_enclave(unsigned long base, unsigned long size);
uintptr_t mcall_sm_destroy_enclave(unsigned long eid);
uintptr_t mcall_sm_run_enclave(unsigned long eid, unsigned long ptr);
uintptr_t mcall_sm_exit_enclave(unsigned long ret);
uintptr_t mcall_sm_not_implemented(unsigned long a0);
#endif

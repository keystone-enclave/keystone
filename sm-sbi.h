#ifndef _KEYSTONE_SBI_H_
#define _KEYSTONE_SBI_H_

#include <stdint.h>
#include <stddef.h>
uintptr_t mcall_sm_create_enclave(unsigned long base, unsigned long size);
uintptr_t mcall_sm_destroy_enclave(unsigned long eid);
uintptr_t mcall_sm_run_enclave(unsigned long eid, unsigned long ptr, unsigned long retptr);
uintptr_t mcall_sm_exit_enclave(unsigned long retval);
uintptr_t mcall_sm_not_implemented(unsigned long a0);
uintptr_t mcall_sm_stop_enclave(unsigned long request);
uintptr_t mcall_sm_resume_enclave(unsigned long eid);
#endif

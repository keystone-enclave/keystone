#ifndef _KEYSTONE_SBI_H_
#define _KEYSTONE_SBI_H_

#include <stdint.h>

uintptr_t mcall_sm_create_enclave();
uintptr_t mcall_sm_destroy_enclave();

#endif

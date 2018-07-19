#ifndef _KEYSTONE_SBI_H_
#define _KEYSTONE_SBI_H_

#include <stdint.h>
#include <stddef.h>
int mcall_sm_create_enclave(unsigned long base, unsigned long size);
int mcall_sm_destroy_enclave(unsigned long eid);
int mcall_sm_copy_from_enclave(unsigned long ptr, unsigned long size);
int mcall_sm_copy_to_enclave(unsigned long ptr, unsigned long size);
#endif

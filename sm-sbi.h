#ifndef _KEYSTONE_SBI_H_
#define _KEYSTONE_SBI_H_

#include <stdint.h>

int mcall_sm_create_enclave(unsigned long base, unsigned long size);
int mcall_sm_destroy_enclave(unsigned long eid);

#endif

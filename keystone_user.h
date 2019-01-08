//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _KEYSTONE_USER_H_
#define _KEYSTONE_USER_H_

#include <linux/types.h>
#include <linux/ioctl.h>
// Linux generic TEE subsystem magic defined in <linux/tee.h>
#define KEYSTONE_IOC_MAGIC  0xa4

// ioctl definition
#define KEYSTONE_IOC_CREATE_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x00, struct keystone_ioctl_create_enclave)
#define KEYSTONE_IOC_DESTROY_ENCLAVE \
  _IOW(KEYSTONE_IOC_MAGIC, 0x01, struct keystone_ioctl_create_enclave)
#define KEYSTONE_IOC_RUN_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x04, struct keystone_ioctl_run_enclave)
#define KEYSTONE_IOC_RESUME_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x05, struct keystone_ioctl_run_enclave)

struct runtime_params_t {
  __u64 runtime_entry;
  __u64 user_entry;
  __u64 untrusted_ptr;
  __u64 untrusted_size;
};

struct keystone_ioctl_create_enclave {
  __u64 eid;
  
  // User ELF
  __u64 user_elf_ptr;
  __u64 user_elf_size;
  __u64 user_stack_size;

  // Runtime ELF
  __u64 runtime_elf_ptr;
  __u64 runtime_elf_size;
  __u64 runtime_stack_size;

  // Runtime Parameters
  struct runtime_params_t params;
};

struct keystone_ioctl_run_enclave {
  __u64 eid;
  __u64 entry;
  __u64 args_ptr;
  __u64 args_size;
  __u64 ret;
};

#endif

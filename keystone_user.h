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
#define KEYSTONE_IOC_ADD_PAGE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x06, struct addr_packed)
#define KEYSTONE_IOC_FINALIZE_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x07, struct keystone_ioctl_create_enclave)
#define KEYSTONE_IOC_UTM_ALLOC \
  _IOR(KEYSTONE_IOC_MAGIC, 0x08, struct addr_packed)
#define KEYSTONE_IOC_UTM_INIT \
  _IOR(KEYSTONE_IOC_MAGIC, 0x09, struct keystone_ioctl_create_enclave)
#define KEYSTONE_IOC_ALLOC_VSPACE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x0a, struct keystone_ioctl_alloc_vspace)

#define RT_NOEXEC 0
#define USER_NOEXEC 1
#define RT_FULL 2
#define USER_FULL 3
#define UTM_FULL 4


struct runtime_params_t {
  __u64 runtime_entry;
  __u64 user_entry;
  __u64 untrusted_ptr;
  __u64 untrusted_size;
};

struct keystone_ioctl_create_enclave {
  __u64 eid;

  //Min pages required
  __u64 min_pages;

  // virtual addresses
  __u64 runtime_vaddr;
  __u64 user_vaddr;

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

struct addr_packed {
  __u64 va;
  __u64 copied;
  __u64 eid;
  __u64 mode;
};

struct keystone_ioctl_alloc_vspace {
  __u64 eid;
  __u64 vaddr;
  __u64 size;
};

#endif

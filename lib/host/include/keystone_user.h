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
#define KEYSTONE_IOC_FINALIZE_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x06, struct keystone_ioctl_create_enclave)
#define KEYSTONE_IOC_UTM_INIT \
  _IOR(KEYSTONE_IOC_MAGIC, 0x07, struct keystone_ioctl_create_enclave)

#define RT_NOEXEC 0
#define USER_NOEXEC 1
#define RT_FULL 2
#define USER_FULL 3
#define UTM_FULL 4

#define MDSIZE 64

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

  __u64 pt_ptr;
  __u64 utm_free_ptr;

  //Used for hash
  __u64 epm_paddr;
  __u64 utm_paddr;
  __u64 runtime_paddr;
  __u64 user_paddr;
  __u64 free_paddr;

  __u64 epm_size;
  __u64 utm_size;

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

struct keystone_hash_enclave {
  __u64 epm_paddr;
  __u64 epm_size;
  __u64 utm_paddr;
  __u64 utm_size;

  __u64 runtime_paddr;
  __u64 user_paddr;
  __u64 free_paddr;

  __u64 untrusted_ptr;
  __u64 untrusted_size;
};


#endif

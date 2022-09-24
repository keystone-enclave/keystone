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

struct runtime_params_t {
  uintptr_t runtime_entry;
  uintptr_t user_entry;
  uintptr_t untrusted_ptr;
  uintptr_t  untrusted_size;
};

struct keystone_ioctl_create_enclave {
  uintptr_t eid;

  //Min pages required
  uintptr_t min_pages;

  // virtual addresses
  uintptr_t runtime_vaddr;
  uintptr_t user_vaddr;

  uintptr_t pt_ptr;
  uintptr_t utm_free_ptr;

  //Used for hash
  uintptr_t epm_paddr;
  uintptr_t utm_paddr;
  uintptr_t runtime_paddr;
  uintptr_t user_paddr;
  uintptr_t free_paddr;

  uintptr_t epm_size;
  uintptr_t utm_size;

  // Runtime Parameters
  struct runtime_params_t params;
};

struct keystone_ioctl_run_enclave {
  uintptr_t eid;
  uintptr_t error;
  uintptr_t value;
};

#endif

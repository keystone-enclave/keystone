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
#define KEYSTONE_IOC_COPY_TO_ENCLAVE \
  _IOW(KEYSTONE_IOC_MAGIC, 0x02, struct keystone_ioctl_enclave_data)
#define KEYSTONE_IOC_COPY_FROM_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x03, struct keystone_ioctl_enclave_data)
#define KEYSTONE_IOC_RUN_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x04, struct keystone_ioctl_run_enclave)
#define KEYSTONE_IOC_RESUME_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x05, struct keystone_ioctl_run_enclave)

struct keystone_ioctl_create_enclave {
  __u64 eid;
  // Enclave App
  __u64 eapp_ptr;
  __u64 eapp_sz;
  __u64 eapp_stack_sz;
  // Runtime
  __u64 runtime_ptr;
  __u64 runtime_sz;
  __u64 runtime_stack_sz;
  // Untrusted Memory
  __u64 untrusted_sz;
};

struct keystone_ioctl_run_enclave {
  __u64 eid;
  __u64 entry;
  __u64 args_ptr;
  __u64 args_size;
  __u64 ret;
};

#endif

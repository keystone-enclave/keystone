#ifndef _KEYSTONE_USER_H_
#define _KEYSTONE_USER_H_

#include <linux/types.h>
#include <linux/ioctl.h>
// Linux generic TEE subsystem magic defined in <linux/tee.h>
#define KEYSTONE_IOC_MAGIC  0xa4

// ioctl definition
#define KEYSTONE_IOC_CREATE_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x00, struct keystone_ioctl_enclave_id)
#define KEYSTONE_IOC_DESTROY_ENCLAVE \
  _IOW(KEYSTONE_IOC_MAGIC, 0x01, struct keystone_ioctl_enclave_id)
#define KEYSTONE_IOC_COPY_TO_ENCLAVE \
  _IOW(KEYSTONE_IOC_MAGIC, 0x02, struct keystone_ioctl_enclave_data)
#define KEYSTONE_IOC_COPY_FROM_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x03, struct keystone_ioctl_enclave_data)
#define KEYSTONE_IOC_RUN_ENCLAVE \
  _IOR(KEYSTONE_IOC_MAGIC, 0x04, struct keystone_ioctl_run_enclave)

struct keystone_ioctl_enclave_id {
  __u64 eid;
  __u64 ptr;
  __u64 size;
};

struct keystone_ioctl_enclave_data {
  __u64 eid;
  __u64 ptr;
  __u64 size;
};

struct keystone_ioctl_run_enclave {
  __u64 eid;
  __u64 ptr;
  __u64 ret;
};

#endif

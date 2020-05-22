#ifndef __KEYSTONE_DEVICE_H__
#define __KEYSTONE_DEVICE_H__

#include <stddef.h>
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <stdarg.h>
#include <assert.h>
#include "common.h"
#include "keystone_user.h"
#include "Params.h"

class KeystoneDevice {
protected:
  int fd;
public:
    KeystoneDevice() {};
    ~KeystoneDevice() {};
    bool initDevice(Params params);
    int create(struct keystone_ioctl_create_enclave *enclp);
    int initUTM(struct keystone_ioctl_create_enclave *enclp);
    int finalize(struct keystone_ioctl_create_enclave *enclp);
    int destroy(struct keystone_ioctl_create_enclave *enclp);
    int run(struct keystone_ioctl_run_enclave *enclp);
    int resume(struct keystone_ioctl_run_enclave *enclp);
    vaddr_t map(vaddr_t addr, size_t size);
};

class MockKeystoneDevice: public KeystoneDevice {
public:
    MockKeystoneDevice() {};
    ~MockKeystoneDevice() {};
    bool initDevice(Params params);
    int create(struct keystone_ioctl_create_enclave *enclp);
    int initUTM(struct keystone_ioctl_create_enclave *enclp);
    int finalize(struct keystone_ioctl_create_enclave *enclp);
    int destroy(struct keystone_ioctl_create_enclave *enclp);
    int run(struct keystone_ioctl_run_enclave *enclp);
    int resume(struct keystone_ioctl_run_enclave *enclp);
    vaddr_t map(vaddr_t addr, size_t size);
};

#endif

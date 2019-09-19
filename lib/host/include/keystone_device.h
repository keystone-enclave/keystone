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
#include "params.h"



class KeystoneDeviceInterface
{
private:
    int fd;
public:
    KeystoneDeviceInterface() {};
    ~KeystoneDeviceInterface() {};
    virtual bool initDevice(Params params) = 0;
    virtual int ioctl_ioc_create_enclave(struct keystone_ioctl_create_enclave *enclp) = 0;
    virtual int ioctl_ioc_utm_init(struct keystone_ioctl_create_enclave *enclp) = 0;
    virtual int ioctl_ioc_finalize_enclave(struct keystone_ioctl_create_enclave *enclp) = 0;
    virtual int ioctl_destroy_enclave(struct keystone_ioctl_create_enclave *enclp) = 0;
    virtual int ioctl_run_enclave(struct keystone_ioctl_run_enclave *enclp) = 0;
    virtual int ioctl_resume_enclave(struct keystone_ioctl_run_enclave *enclp) = 0;
    virtual int getFD() = 0;
};

class KeystoneDevice: public KeystoneDeviceInterface{
private:
    int fd;
public:
    KeystoneDevice() {};
    ~KeystoneDevice() {};
    bool initDevice(Params params);
    int ioctl_ioc_create_enclave(struct keystone_ioctl_create_enclave *enclp);
    int ioctl_ioc_utm_init(struct keystone_ioctl_create_enclave *enclp);
    int ioctl_ioc_finalize_enclave(struct keystone_ioctl_create_enclave *enclp);
    int ioctl_destroy_enclave(struct keystone_ioctl_create_enclave *enclp);
    int ioctl_run_enclave(struct keystone_ioctl_run_enclave *enclp);
    int ioctl_resume_enclave(struct keystone_ioctl_run_enclave *enclp);
    int getFD();

};

class MockKeystoneDevice: public KeystoneDeviceInterface{
private:
    int fd;
public:
    MockKeystoneDevice() {};
    ~MockKeystoneDevice() {};
    bool initDevice(Params params);
    int ioctl_ioc_create_enclave(struct keystone_ioctl_create_enclave *enclp);
    int ioctl_ioc_utm_init(struct keystone_ioctl_create_enclave *enclp);
    int ioctl_ioc_finalize_enclave(struct keystone_ioctl_create_enclave *enclp);
    int ioctl_destroy_enclave(struct keystone_ioctl_create_enclave *enclp);
    int ioctl_run_enclave(struct keystone_ioctl_run_enclave *enclp);
    int ioctl_resume_enclave(struct keystone_ioctl_run_enclave *enclp);
    int getFD();
};


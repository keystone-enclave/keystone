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
#include "KeystoneError.h"

class KeystoneDevice {
protected:
    int fd;
    int eid;
    unsigned long physAddr;
private:
    KeystoneError __run(bool resume);
public:
    unsigned long getPhysAddr() { return physAddr; }

    KeystoneDevice();
    ~KeystoneDevice() {};
    bool initDevice(Params params);
    KeystoneError create(unsigned long minPages);
    vaddr_t initUTM(size_t size);
    KeystoneError finalize(uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr, struct runtime_params_t params);
    KeystoneError destroy();
    KeystoneError run();
    KeystoneError resume();
    vaddr_t map(vaddr_t addr, size_t size);
};

class MockKeystoneDevice: public KeystoneDevice {
public:
    MockKeystoneDevice() {};
    ~MockKeystoneDevice() {};
    bool initDevice(Params params);
    KeystoneError create(unsigned long minPages);
    vaddr_t initUTM(size_t size);
    KeystoneError finalize(uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr, struct runtime_params_t params);
    KeystoneError destroy();
    KeystoneError run();
    KeystoneError resume();
    vaddr_t map(vaddr_t addr, size_t size);
};

#endif

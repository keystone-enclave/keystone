//
// Created by Alex Thomas on 8/17/19.
//

#include "KeystoneDevice.h"
#include <sys/mman.h>

int KeystoneDevice::create(struct keystone_ioctl_create_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, enclp);
}

int KeystoneDevice::initUTM(struct keystone_ioctl_create_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_UTM_INIT, enclp);

}

int KeystoneDevice::finalize(struct keystone_ioctl_create_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_FINALIZE_ENCLAVE, enclp);
}

int KeystoneDevice::destroy(struct keystone_ioctl_create_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, enclp);
}

int KeystoneDevice::run(struct keystone_ioctl_run_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_RUN_ENCLAVE, enclp);
}

int KeystoneDevice::resume(struct keystone_ioctl_run_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_RESUME_ENCLAVE, enclp);
}

vaddr_t KeystoneDevice::map(vaddr_t addr, size_t size)
{
  assert(fd >= 0);
  return (vaddr_t)
    mmap(NULL,
         size,
         PROT_READ | PROT_WRITE,
         MAP_SHARED,
         fd,
         addr);
}

bool KeystoneDevice::initDevice(Params params)
{
    /* open device driver */
    fd = open(KEYSTONE_DEV_PATH, O_RDWR);
    if (fd < 0) {
      PERROR("cannot open device file");
      return false;
    }
  return true;
}

int MockKeystoneDevice::create(struct keystone_ioctl_create_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::initUTM(struct keystone_ioctl_create_enclave *enclp)
{
  return 0;

}

int MockKeystoneDevice::finalize(struct keystone_ioctl_create_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::destroy(struct keystone_ioctl_create_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::run(struct keystone_ioctl_run_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::resume(struct keystone_ioctl_run_enclave *enclp)
{
  return 0;
}

bool MockKeystoneDevice::initDevice(Params params)
{
  return true;
}


//
// Created by Alex Thomas on 8/17/19.
//

#include "keystone_device.h"

int KeystoneDevice::ioctl_ioc_create_enclave(struct keystone_ioctl_create_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, enclp);
}

int KeystoneDevice::ioctl_ioc_utm_init(struct keystone_ioctl_create_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_UTM_INIT, enclp);

}

int KeystoneDevice::ioctl_ioc_finalize_enclave(struct keystone_ioctl_create_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_FINALIZE_ENCLAVE, enclp);
}

int KeystoneDevice::ioctl_destroy_enclave(struct keystone_ioctl_create_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, enclp);
}

int KeystoneDevice::ioctl_run_enclave(struct keystone_ioctl_run_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_RUN_ENCLAVE, enclp);
}

int KeystoneDevice::ioctl_resume_enclave(struct keystone_ioctl_run_enclave *enclp)
{
  return ioctl(fd, KEYSTONE_IOC_RESUME_ENCLAVE, enclp);
}

int KeystoneDevice::getFD(){
  return fd;
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

int MockKeystoneDevice::ioctl_ioc_create_enclave(struct keystone_ioctl_create_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::ioctl_ioc_utm_init(struct keystone_ioctl_create_enclave *enclp)
{
  return 0;

}

int MockKeystoneDevice::ioctl_ioc_finalize_enclave(struct keystone_ioctl_create_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::ioctl_destroy_enclave(struct keystone_ioctl_create_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::ioctl_run_enclave(struct keystone_ioctl_run_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::ioctl_resume_enclave(struct keystone_ioctl_run_enclave *enclp)
{
  return 0;
}

int MockKeystoneDevice::getFD(){
  return 5;
}

bool MockKeystoneDevice::initDevice(Params params)
{
  return true;
}


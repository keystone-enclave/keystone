#include "keystone.h"
#include "keystone_user.h"
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <cerrno>

Keystone::Keystone()
{
  eid = -1;
}

Keystone::~Keystone()
{
  destroy();
}

keystone_status_t Keystone::init()
{
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if(fd < 0)
    perror("cannot open device file " KEYSTONE_DEV_PATH "\n");

  struct keystone_ioctl_enclave_id enclp;
  int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);

  if(ret < 0){
    perror("ioctl() failed\n");
    return KEYSTONE_ERROR;
  }

  eid = enclp.eid;

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::destroy()
{
  struct keystone_ioctl_enclave_id enclp;
  enclp.eid = eid;
  int ret = ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, &enclp);

  if(ret < 0) {
    perror("ioctl() failed\n");
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::copy_from_encl(void* ptr, size_t size)
{
  int ret;
  
  struct keystone_ioctl_enclave_data data;
  data.ptr = (unsigned long) ptr;
  data.size = (unsigned long) size;

  ret = ioctl(fd, KEYSTONE_IOC_COPY_FROM_ENCLAVE, &data);
  if(ret < 0) {
    perror("ioctl() failed\n");
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::copy_to_encl(void* ptr, size_t size)
{
  int ret;

  struct keystone_ioctl_enclave_data data;
  data.ptr = (unsigned long) ptr;
  data.size = (unsigned long) size;

  ret = ioctl(fd, KEYSTONE_IOC_COPY_TO_ENCLAVE, &data);
  if(ret < 0)
  {
    perror("ioctl() failed\n");
    return KEYSTONE_ERROR;
  }
  return KEYSTONE_SUCCESS;
}

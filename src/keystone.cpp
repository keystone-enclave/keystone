#include "keystone.h"
#include "abi.h"
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
{}

keystone_status_t Keystone::init()
{
  int fd;
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if(fd < 0)
    perror("cannot open device file " KEYSTONE_DEV_PATH "\n");

  int ret = ioctl(fd, KEYSTONE_CREATE_ENCLAVE, &eid);
  if(ret < 0){
    perror("ioctl() failed\n");
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::copy_from_encl(void* ptr, size_t size)
{
  int ret;
  ret = ABI_CALL_2(KEYSTONE_COPY_FROM_ENCLAVE, ptr, size);
  if(ret < 0)
  {
    std::cout<<"abi call failed\n"<<std::strerror(-ret)<<std::endl;
    return KEYSTONE_ERROR;
  }
  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::copy_to_encl(void* ptr, size_t size)
{
  int ret;
  ret = ABI_CALL_2(KEYSTONE_COPY_TO_ENCLAVE, ptr, size);
  if(ret < 0)
  {
    std::cout<<"abi call failed\n"<<std::strerror(-ret)<<std::endl;
    return KEYSTONE_ERROR;
  }
  return KEYSTONE_SUCCESS;
}



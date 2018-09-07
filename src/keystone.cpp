#include <sys/stat.h>
#include <sys/mman.h>
#include "keystone.h"
#include "keystone_user.h"
#include "abi.h"

Keystone::Keystone()
{
  eid = -1;
}

Keystone::~Keystone()
{
  destroy();
}

keystone_status_t Keystone::init(void* ptr, size_t size)
{
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if(fd < 0)
    PERROR("cannot open device file");

  struct keystone_ioctl_enclave_id enclp;

  enclp.ptr = (unsigned long) ptr;
  enclp.size = (unsigned long) size;
  int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);

  if(ret < 0) {
    PERROR("failed to create enclave - ioctl() failed");
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
    PERROR("failed to destroy enclave - ioctl() failed");
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::copyFromEnclave(void* ptr, size_t size)
{
  int ret;
  
  struct keystone_ioctl_enclave_data data;
  data.eid = eid;
  data.ptr = (unsigned long) ptr;
  data.size = (unsigned long) size;

  ret = ioctl(fd, KEYSTONE_IOC_COPY_FROM_ENCLAVE, &data);
  if(ret < 0) {
    PERROR("failed to copy from enclave - ioctl() failed\n");
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::copyToEnclave(void* ptr, size_t size)
{
  int ret;

  struct keystone_ioctl_enclave_data data;
  data.eid = eid;
  data.ptr = (unsigned long) ptr;
  data.size = (unsigned long) size;

  ret = ioctl(fd, KEYSTONE_IOC_COPY_TO_ENCLAVE, &data);
  if(ret < 0)
  {
    PERROR("failed to copy to enclave - ioctl() failed\n");
    return KEYSTONE_ERROR;
  }
  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::run(void* ptr)
{
  int	ret;
  struct keystone_ioctl_run_enclave run;
  run.eid = eid;
  run.ptr = (unsigned long) ptr;

  ret = ioctl(fd, KEYSTONE_IOC_RUN_ENCLAVE, &run);

  printf("%ld\n", run.ret);
	if(ret < 0)
  {
		PERROR("failed to run enclave - ioctl() failed");
    return KEYSTONE_ERROR;
	}

	return KEYSTONE_SUCCESS;
}


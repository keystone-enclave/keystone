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

keystone_status_t Keystone::init(void* ptr, size_t code_size, size_t mem_size)
{
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if(fd < 0)
    PERROR("cannot open device file");

  struct keystone_ioctl_enclave_id enclp;

  enclp.ptr = (unsigned long) ptr;
  enclp.code_size = (unsigned long) code_size;
  enclp.mem_size = (unsigned long) mem_size;
  int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);

  if(ret < 0) {
    PERROR("failed to create enclave - ioctl() failed");
    return KEYSTONE_ERROR;
  }

  eid = enclp.eid;
  this->ptr = ptr;
  
  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::init(char* filepath, size_t mem_size)
{
  FILE* app_file;
  void* app_code_buffer;
  size_t res, code_size;
  int ret;
  
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if(fd < 0){
    PERROR("cannot open device file");
    goto err_close;
  }
  // Open up the target file and read it into memory
  app_file = fopen(filepath,"r");
  if(app_file == NULL){
    PERROR("Unable to open enclave app file. - fopen() failed");
    goto err_ret;
  }

  fseek(app_file, 0, SEEK_END);
  code_size=ftell(app_file);
  app_code_buffer = malloc(code_size);
  if(app_code_buffer == NULL){
    PERROR("Unable to allocate space to read app file. - malloc() failed");
    goto err_close;
  }
  rewind(app_file);
  res = fread(app_code_buffer, 1, code_size, app_file);

  if(res != code_size){
    PERROR("Enclave application file loading error. - fread() failed");
    goto err_all;
  }
  
  struct keystone_ioctl_enclave_id enclp;

  enclp.ptr = (unsigned long) app_code_buffer;
  enclp.code_size = (unsigned long) code_size;
  enclp.mem_size = (unsigned long) mem_size;
  ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);

  if(ret < 0) {
    PERROR("failed to create enclave - ioctl() failed");
    goto err_all;
  }

  eid = enclp.eid;
  this->ptr = app_code_buffer;
  
  return KEYSTONE_SUCCESS;
  
 err_all:
  free(app_code_buffer);
 err_close:
  fclose(app_file);
 err_ret:
  return KEYSTONE_ERROR;
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

keystone_status_t Keystone::run()
{
  int	ret;
  struct keystone_ioctl_run_enclave run;
  run.eid = eid;
  run.ptr = (unsigned long) this->ptr;

  ret = ioctl(fd, KEYSTONE_IOC_RUN_ENCLAVE, &run);

  printf("%ld\n", run.ret);
	if(ret < 0)
  {
		PERROR("failed to run enclave - ioctl() failed");
    return KEYSTONE_ERROR;
	}

	return KEYSTONE_SUCCESS;
}


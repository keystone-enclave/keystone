#include <sys/stat.h>
#include <sys/mman.h>
#include "keystone.h"
#include "keystone_user.h"

Keystone::Keystone()
{
  eid = -1;
}

Keystone::~Keystone()
{
  destroy();
}

keystone_status_t Keystone::init_elf(char* filepath, size_t mem_size, unsigned long usr_entry_ptr){

  FILE* app_file;
  void* app_code_buffer;
  size_t res, code_size;
  int ret;

  this->entry_ptr = usr_entry_ptr;
  
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

  //printf("Enclave info: ptr:%p code_sz:%ul mem_sz:%ul\n",app_code_buffer, code_size, mem_size);
  ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);

  if(ret) {
    printf("failed to create enclave - ioctl() failed: %d", ret);
    goto err_all;
  }

  printf("Created enclave\n");
  
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

  if(ret) {
    printf("failed to destroy enclave - ioctl() failed: %d", ret);
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::run()
{
  int	ret;
  struct keystone_ioctl_run_enclave run;
  run.eid = eid;

  run.ptr = this->entry_ptr;

  printf("Starting enclave!\n");

  ret = ioctl(fd, KEYSTONE_IOC_RUN_ENCLAVE, &run);

  printf("%ld\n", run.ret);
  if(ret)
    {
      printf("failed to run enclave - ioctl() failed: %d", ret);
      return KEYSTONE_ERROR;
    }
  
  return KEYSTONE_SUCCESS;
}


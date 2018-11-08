#include <sys/stat.h>
#include <sys/mman.h>
#include "keystone.h"
#include "keystone_user.h"

Keystone::Keystone()
{
  runtimeFile = NULL;
  enclaveFile = NULL;
  eid = -1;
}

Keystone::~Keystone()
{
  delete runtimeFile;
  delete enclaveFile;
  destroy();
}

/* FIXME: someone should parse this */
#define RUNTIME_ENTRY 0xffffffffc0000000
keystone_status_t Keystone::init(char* eapppath, char* runtimepath, size_t stack_size, size_t untrusted_size, unsigned long usr_entry_ptr)
{
  if(runtimeFile || enclaveFile)
  {
    ERROR("ELF files already initialized");
    return KEYSTONE_ERROR;
  }

  runtimeFile = new ELFFile(runtimepath);
  enclaveFile = new ELFFile(eapppath);
  
  /* these should be parsed by ELF lib */
  runtimeFile->setEntry(RUNTIME_ENTRY);
  enclaveFile->setEntry(usr_entry_ptr); 
 
  /* open device driver */
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if(fd < 0){
    PERROR("cannot open device file");
    return KEYSTONE_ERROR;
  }
  // Open up the target file and read it into memory
  
  if(!runtimeFile->isValid())
  {
    ERROR("runtime file is not valid");
    return KEYSTONE_ERROR;
  }
  if(!enclaveFile->isValid())
  {
    ERROR("enclave file is not valid");
    return KEYSTONE_ERROR;
  }
  
  struct keystone_ioctl_create_enclave enclp;

  enclp.eapp_ptr = (unsigned long) enclaveFile->getPtr();
  enclp.eapp_sz = (unsigned long) enclaveFile->getSize();
  enclp.eapp_stack_sz = (unsigned long) stack_size;
  enclp.eapp_entry = (unsigned long) usr_entry_ptr; 
  enclp.runtime_ptr = (unsigned long) runtimeFile->getPtr();
  enclp.runtime_sz = (unsigned long) runtimeFile->getSize();
  enclp.runtime_stack_sz = (unsigned long) 4096*2;
  enclp.runtime_entry = (unsigned long) RUNTIME_ENTRY;
  enclp.untrusted_sz = untrusted_size;

  //printf("Enclave info: ptr:%p code_sz:%ul mem_sz:%ul\n",app_code_buffer, code_size, stack_size);
  int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);
  if(ret) {
    ERROR("failed to create enclave - ioctl() failed: %d", ret);
    return KEYSTONE_ERROR;
  }
  eid = enclp.eid;

  return mapUntrusted(untrusted_size);
}

keystone_status_t Keystone::mapUntrusted(size_t size)
{
  shared_buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  if (shared_buffer == NULL)
  {
    return KEYSTONE_ERROR;
  }
  shared_buffer_size = size;

  return KEYSTONE_SUCCESS;
}

keystone_status_t Keystone::destroy()
{
  struct keystone_ioctl_create_enclave enclp;
  enclp.eid = eid;
  int ret = ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, &enclp);

  if(ret) {
    ERROR("failed to destroy enclave - ioctl() failed: %d", ret);
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

#define KEYSTONE_ENCLAVE_EDGE_CALL_HOST  11

keystone_status_t Keystone::run()
{
  int	ret;
  struct keystone_ioctl_run_enclave run;
  run.eid = eid;

  ret = ioctl(fd, KEYSTONE_IOC_RUN_ENCLAVE, &run);
  while (ret == KEYSTONE_ENCLAVE_EDGE_CALL_HOST)
  {
    /* enclave is stopped in the middle. */
    if (oFuncDispatch != NULL) {
      oFuncDispatch(getSharedBuffer(), getSharedBufferSize());
    }
    ret = ioctl(fd, KEYSTONE_IOC_RESUME_ENCLAVE, &run);
  }
  if(ret)
  {
    ERROR("failed to run enclave - ioctl() failed: %d", ret);
    return KEYSTONE_ERROR;
  }

  return KEYSTONE_SUCCESS;
}

void* Keystone::getSharedBuffer()
{
  return shared_buffer;
}

size_t Keystone::getSharedBufferSize()
{
  return shared_buffer_size;
}

keystone_status_t Keystone::registerOcallDispatch(OcallFunc func)
{
  oFuncDispatch = func;
}

//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
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

keystone_status_t Keystone::init(char* eapppath, char* runtimepath, Params params)
{
  if(runtimeFile || enclaveFile)
  {
    ERROR("ELF files already initialized");
    return KEYSTONE_ERROR;
  }

  runtimeFile = new ELFFile(runtimepath);
  enclaveFile = new ELFFile(eapppath);
  
  /* these should be parsed by ELF lib */
 
  /* open device driver */
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if(fd < 0){
    PERROR("cannot open device file");
    return KEYSTONE_ERROR;
  }
  
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

  /* Call Keystone Driver */
  struct keystone_ioctl_create_enclave enclp;

  enclp.user_elf_ptr = (unsigned long) enclaveFile->getPtr();
  enclp.user_elf_size = (unsigned long) enclaveFile->getSize();
  enclp.runtime_elf_ptr = (unsigned long) runtimeFile->getPtr();
  enclp.runtime_elf_size = (unsigned long) runtimeFile->getSize();

  enclp.user_stack_size = (unsigned long) params.getEnclaveStack();
  enclp.runtime_stack_size = (unsigned long) params.getRuntimeStack();

  enclp.params.runtime_entry = (unsigned long) params.getRuntimeEntry();
  enclp.params.user_entry = (unsigned long) params.getEnclaveEntry(); 
  enclp.params.untrusted_ptr = (unsigned long) params.getUntrustedMem();
  enclp.params.untrusted_size = (unsigned long) params.getUntrustedSize();

  int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &enclp);
  if(ret) {
    ERROR("failed to create enclave - ioctl() failed: %d", ret);
    return KEYSTONE_ERROR;
  }
  eid = enclp.eid;

  return mapUntrusted(params.getUntrustedSize());
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
      oFuncDispatch(getSharedBuffer());
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
  return KEYSTONE_SUCCESS;
}

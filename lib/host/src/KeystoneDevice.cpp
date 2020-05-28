//
// Created by Alex Thomas on 8/17/19.
//

#include "KeystoneDevice.h"
#include <sys/mman.h>

KeystoneDevice::KeystoneDevice()
{
  eid = -1;
}

KeystoneError KeystoneDevice::create(unsigned long minPages)
{
  struct keystone_ioctl_create_enclave encl;
  encl.min_pages = minPages;

  int ret = ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &encl);
  if (ret)
  {
    perror("ioctl error");
    eid = -1;
    return KeystoneError::IoctlErrorCreate;
  }

  eid = encl.eid;
  physAddr = encl.pt_ptr;

  return KeystoneError::Success;
}

vaddr_t KeystoneDevice::initUTM(size_t size)
{
  struct keystone_ioctl_create_enclave encl;
  encl.eid = eid;
  encl.params.untrusted_size = size;
  int ret;
  if (ret = ioctl(fd, KEYSTONE_IOC_UTM_INIT, &encl))
  {
    return 0;
  }

  return encl.utm_free_ptr;
}

KeystoneError
KeystoneDevice::finalize(
    uintptr_t runtimePhysAddr,
    uintptr_t eappPhysAddr,
    uintptr_t freePhysAddr,
    struct runtime_params_t params)
{
  struct keystone_ioctl_create_enclave encl;
  encl.eid = eid;
  encl.runtime_paddr = runtimePhysAddr;
  encl.user_paddr = eappPhysAddr;
  encl.free_paddr = freePhysAddr;
  encl.params = params;

  int ret;
  if(ret = ioctl(fd, KEYSTONE_IOC_FINALIZE_ENCLAVE, &encl))
  {
    perror("ioctl error");
    return KeystoneError::IoctlErrorFinalize;
  }
  return KeystoneError::Success;
}

KeystoneError
KeystoneDevice::destroy()
{
  struct keystone_ioctl_create_enclave encl;
  encl.eid = eid;

  /* if the enclave has never created */
  if (eid < 0)
  {
    return KeystoneError::Success;
  }

  int ret;
  if(ret = ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, &encl))
  {
    perror("ioctl error");
    return KeystoneError::IoctlErrorDestroy;
  }

  return KeystoneError::Success;
}

KeystoneError
KeystoneDevice::__run(bool resume)
{
  struct keystone_ioctl_run_enclave encl;
  encl.eid = eid;

  KeystoneError error;
  unsigned long request;

  if (resume) {
    error = KeystoneError::IoctlErrorResume;
    request = KEYSTONE_IOC_RESUME_ENCLAVE;
  } else {
    error = KeystoneError::IoctlErrorRun;
    request = KEYSTONE_IOC_RUN_ENCLAVE;
  }

  int ret;
  ret = ioctl(fd, request, &encl);

  switch(ret)
  {
    case KEYSTONE_ENCLAVE_EDGE_CALL_HOST:
      return KeystoneError::EdgeCallHost;
    case KEYSTONE_ENCLAVE_INTERRUPTED:
      return KeystoneError::EnclaveInterrupted;
    case KEYSTONE_ENCLAVE_DONE:
      return KeystoneError::Success;
    default:
      perror("ioctl error");
      return error;
  }
}

KeystoneError
KeystoneDevice::run()
{
  return __run(false);
}

KeystoneError
KeystoneDevice::resume()
{
  return __run(true);
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

KeystoneError MockKeystoneDevice::create(unsigned long minPages)
{
  eid = -1;
  return KeystoneError::Success;
}

vaddr_t MockKeystoneDevice::initUTM(size_t size)
{
  return 0;
}

KeystoneError MockKeystoneDevice::finalize(uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr, struct runtime_params_t params)
{
  return KeystoneError::Success;
}

KeystoneError
MockKeystoneDevice::destroy()
{
  return KeystoneError::Success;
}

KeystoneError
MockKeystoneDevice::run()
{
  return KeystoneError::Success;
}

KeystoneError
MockKeystoneDevice::resume()
{
  return KeystoneError::Success;
}

bool MockKeystoneDevice::initDevice(Params params)
{
  return true;
}


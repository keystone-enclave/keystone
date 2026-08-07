//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "KeystoneDevice.hpp"
#include <sys/mman.h>

namespace Keystone {

KeystoneDevice::KeystoneDevice() { eid = -1; }

Error
KeystoneDevice::create(uint64_t minPages) {
  struct keystone_ioctl_create_enclave encl;
  encl.min_pages = minPages;

  if (ioctl(fd, KEYSTONE_IOC_CREATE_ENCLAVE, &encl)) {
    perror("ioctl error");
    eid = -1;
    return Error::IoctlErrorCreate;
  }

  eid         = encl.eid;
  epmPhysAddr = encl.epm_paddr;
  epmSize     = encl.epm_size;
  mapEpm();

  return Error::Success;
}

Error
KeystoneDevice::initUTM(size_t size) {
  struct keystone_ioctl_create_enclave encl;
  encl.eid      = eid;
  encl.utm_size = size;
  if (ioctl(fd, KEYSTONE_IOC_UTM_INIT, &encl)) {
    return Error::DeviceError;
  }
  utmPhysAddr = encl.utm_paddr;
  utmSize = size;

  return Error::Success;
}

Error
KeystoneDevice::finalize(uintptr_t freeOffset) {
  struct keystone_ioctl_create_enclave encl;
  encl.eid            = eid;
  encl.free_offset    = freeOffset;

  if (ioctl(fd, KEYSTONE_IOC_FINALIZE_ENCLAVE, &encl)) {
    perror("ioctl error");
    return Error::IoctlErrorFinalize;
  }
  finalizeDone = true;
  return Error::Success;
}

Error
KeystoneDevice::destroy() {
  struct keystone_ioctl_create_enclave encl;
  encl.eid = eid;

  /* if the enclave has never created */
  if (eid < 0) {
    return Error::Success;
  }

  if (ioctl(fd, KEYSTONE_IOC_DESTROY_ENCLAVE, &encl)) {
    perror("ioctl error");
    return Error::IoctlErrorDestroy;
  }

  return Error::Success;
}

Error
KeystoneDevice::__run(bool resume, uintptr_t* ret) {
  struct keystone_ioctl_run_enclave encl;
  encl.eid = eid;

  Error error;
  uint64_t request;

  if (resume) {
    error   = Error::IoctlErrorResume;
    request = KEYSTONE_IOC_RESUME_ENCLAVE;
  } else {
    error   = Error::IoctlErrorRun;
    request = KEYSTONE_IOC_RUN_ENCLAVE;
  }

  if (ioctl(fd, request, &encl)) {
    return error;
  }

  switch (encl.error) {
    case SBI_ERR_SM_ENCLAVE_EDGE_CALL_HOST:
      return Error::EdgeCallHost;
    case SBI_ERR_SM_ENCLAVE_INTERRUPTED:
      return Error::EnclaveInterrupted;
    case SBI_ERR_SM_ENCLAVE_SUCCESS:
      if (ret) {
        *ret = encl.value;
      }
      return Error::Success;
    default:
      ERROR(
          "Unknown SBI error (%d) returned by %s_enclave\n", encl.error,
          resume ? "resume" : "run");
      return error;
  }
}

Error
KeystoneDevice::run(uintptr_t* ret) {
  return __run(false, ret);
}

Error
KeystoneDevice::resume(uintptr_t* ret) {
  return __run(true, ret);
}

uintptr_t
KeystoneDevice::map(uintptr_t addr, size_t size) {
  assert(fd >= 0);
  void* ret;
  ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, addr);
  assert(ret != MAP_FAILED);
  assert(ret != 0);
  return (uintptr_t) ret;
}

Error
KeystoneDevice::mapEpm() {
  assert(!finalizeDone);
  if (!epmPhysAddr || !epmSize) {
    return Error::DeviceError;
  }
  epmVirtAddr = map(0, epmSize);
  return Error::Success;
}

Error
KeystoneDevice::mapUtm() {
  assert(finalizeDone);
  if (!utmPhysAddr || !utmSize) {
    return Error::DeviceError;
  }
  utmVirtAddr = map(0, utmSize);
  return Error::Success;
}

Error
KeystoneDevice::initDevice(Params params) { // TODO: why does this need params
  /* open device driver */
  fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if (fd < 0) {
    PERROR("cannot open device file");
    return Error::DeviceInitFailure;
  }
  return Error::Success;
}

}  // namespace Keystone

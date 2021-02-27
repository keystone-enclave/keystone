//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

#include <assert.h>
#include <fcntl.h>
#include <stdarg.h>
#include <stddef.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include "./common.h"
#include "./keystone_user.h"
#include "Error.hpp"
#include "Params.hpp"

namespace Keystone {

class KeystoneDevice {
 protected:
  int eid;
  uintptr_t physAddr;

 private:
  int fd;
  Error __run(bool resume, uintptr_t* ret);

 public:
  virtual uintptr_t getPhysAddr() { return physAddr; }

  KeystoneDevice();
  virtual ~KeystoneDevice() {}
  virtual bool initDevice(Params params);
  virtual Error create(uint64_t minPages);
  virtual uintptr_t initUTM(size_t size);
  virtual Error finalize(
      uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr,
      struct runtime_params_t params);
  virtual Error destroy();
  virtual Error run(uintptr_t* ret);
  virtual Error resume(uintptr_t* ret);
  virtual void* map(uintptr_t addr, size_t size);
};

class MockKeystoneDevice : public KeystoneDevice {
 private:
  /* allocated buffer with map() */
  void* sharedBuffer;

 public:
  MockKeystoneDevice() {}
  ~MockKeystoneDevice();
  bool initDevice(Params params);
  Error create(uint64_t minPages);
  uintptr_t initUTM(size_t size);
  Error finalize(
      uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr,
      struct runtime_params_t params);
  Error destroy();
  Error run(uintptr_t* ret);
  Error resume(uintptr_t* ret);
  void* map(uintptr_t addr, size_t size);
};

}  // namespace Keystone

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
#include <cstdint>
#include <iostream>
#include "./common.h"
#include "./keystone_user.h"
#include "Error.hpp"
#include "Params.hpp"

namespace Keystone {

class KeystoneDevice {
 protected:
  int eid{ -1 };
  std::uintptr_t physAddr;

 private:
  int fd;
  Error __run(bool resume, std::uintptr_t* ret);

 public:
  virtual std::uintptr_t getPhysAddr() { return physAddr; }

  virtual ~KeystoneDevice() {}
  virtual bool initDevice(Params params);
  virtual Error create(std::uint64_t minPages);
  virtual std::uintptr_t initUTM(std::size_t size);
  virtual Error finalize(
      std::uintptr_t runtimePhysAddr, std::uintptr_t eappPhysAddr, std::uintptr_t freePhysAddr,
      struct runtime_params_t params);
  virtual Error destroy();
  virtual Error run(std::uintptr_t* ret);
  virtual Error resume(std::uintptr_t* ret);
  virtual void* map(std::uintptr_t addr, std::size_t size);
};

class MockKeystoneDevice : public KeystoneDevice {
 private:
  /* allocated buffer with map() */
  void* sharedBuffer;

 public:
  MockKeystoneDevice() {}
  ~MockKeystoneDevice();
  bool initDevice(Params params);
  Error create(std::uint64_t minPages);
  std::uintptr_t initUTM(std::size_t size);
  Error finalize(
      std::uintptr_t runtimePhysAddr, std::uintptr_t eappPhysAddr, std::uintptr_t freePhysAddr,
      struct runtime_params_t params);
  Error destroy();
  Error run(std::uintptr_t* ret);
  Error resume(std::uintptr_t* ret);
  void* map(std::uintptr_t addr, std::size_t size);
};

}  // namespace Keystone

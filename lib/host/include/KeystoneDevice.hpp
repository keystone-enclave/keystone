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
#include "KeystoneError.hpp"
#include "Params.hpp"

class KeystoneDevice {
 protected:
  int fd;
  int eid;
  uintptr_t physAddr;

 private:
  KeystoneError __run(bool resume);

 public:
  uintptr_t getPhysAddr() { return physAddr; }

  KeystoneDevice();
  ~KeystoneDevice() {}
  bool initDevice(Params params);
  KeystoneError create(uint64_t minPages);
  vaddr_t initUTM(size_t size);
  KeystoneError finalize(
      uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr,
      struct runtime_params_t params);
  KeystoneError destroy();
  KeystoneError run();
  KeystoneError resume();
  void* map(vaddr_t addr, size_t size);
};

class MockKeystoneDevice : public KeystoneDevice {
 public:
  MockKeystoneDevice() {}
  ~MockKeystoneDevice() {}
  bool initDevice(Params params);
  KeystoneError create(uint64_t minPages);
  vaddr_t initUTM(size_t size);
  KeystoneError finalize(
      uintptr_t runtimePhysAddr, uintptr_t eappPhysAddr, uintptr_t freePhysAddr,
      struct runtime_params_t params);
  KeystoneError destroy();
  KeystoneError run();
  KeystoneError resume();
  void* map(vaddr_t addr, size_t size);
};

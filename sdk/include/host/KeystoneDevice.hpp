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
#include "Error.hpp"
#include "Params.hpp"
#include "shared/keystone_user.h"

namespace Keystone {

class KeystoneDevice {
 protected:
  int eid = 0;
  bool finalizeDone = false;
  uintptr_t epmPhysAddr = 0, epmSize = 0, epmVirtAddr = 0;
  uintptr_t utmPhysAddr = 0, utmSize = 0, utmVirtAddr = 0;

 private:
  int fd;
  Error __run(bool resume, uintptr_t* ret);
  // map maps epm before finalize and utm after.
  virtual uintptr_t map(uintptr_t addr, size_t size);

 public:
  uintptr_t getEpmPhysAddr() { return epmPhysAddr; }
  uintptr_t getEpmSize() { return epmSize; }
  uintptr_t getEpmVirtAddr() { return epmVirtAddr; }
  uintptr_t getUtmPhysAddr() { return utmPhysAddr; }
  uintptr_t getUtmSize() { return utmSize; }
  uintptr_t getUtmVirtAddr() { return utmVirtAddr; }

  KeystoneDevice();
  virtual ~KeystoneDevice() {}
  virtual Error initDevice(Params params);
  virtual Error create(uint64_t minPages);
  virtual Error initUTM(size_t size);
  virtual Error finalize(uintptr_t freeOffset);
  virtual Error destroy();
  virtual Error run(uintptr_t* ret);
  virtual Error resume(uintptr_t* ret);
  // pre-finalize only
  virtual Error mapEpm();
  // post-finalize only
  virtual Error mapUtm();
};

}  // namespace Keystone

//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
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
#include "KeystoneDevice.hpp"
#include "hash_util.hpp"

namespace Keystone {

class Memory {
 public:
  Memory();
  ~Memory() {}
  virtual void init(
      KeystoneDevice* dev, uintptr_t phys_addr, size_t min_pages)  = 0;
  virtual uintptr_t readMem(uintptr_t src, size_t size)            = 0;
  virtual void writeMem(uintptr_t src, uintptr_t dst, size_t size) = 0;
  virtual uintptr_t allocMem(size_t size)                          = 0;
  virtual uintptr_t allocUtm(size_t size)                          = 0;
  size_t epmAllocVspace(uintptr_t addr, size_t num_pages);
  uintptr_t allocPages(size_t size); 


  // getters to be deprecated
  uintptr_t getStartAddr() { return startAddr; }
  uintptr_t getCurrentOffset() { return epmFreeList; }
  uintptr_t getCurrentEPMAddress() { return epmFreeList + startAddr; }

  void startRuntimeMem();
  void startEappMem();
  void startFreeMem();

  void incrementEPMFreeList();

  uintptr_t getRuntimePhysAddr() { return runtimePhysAddr; }
  uintptr_t getEappPhysAddr() { return eappPhysAddr; }
  uintptr_t getFreePhysAddr() { return freePhysAddr; }
  uintptr_t getUTMPhysAddr() { return utmPhysAddr; }

  KeystoneDevice* pDevice;
  size_t epmSize;
  uintptr_t epmFreeList;
  uintptr_t startAddr;

  // Keystone Device runtime params
  uintptr_t runtimePhysAddr;
  uintptr_t eappPhysAddr;
  uintptr_t freePhysAddr;
  uintptr_t utmPhysAddr;
  uintptr_t untrustedPtr;
  uintptr_t untrustedSize;
};

class PhysicalEnclaveMemory : public Memory {
 public:
  PhysicalEnclaveMemory() {}
  ~PhysicalEnclaveMemory() {}
  void init(KeystoneDevice* dev, uintptr_t phys_addr, size_t min_pages);
  uintptr_t readMem(uintptr_t src, size_t size);
  void writeMem(uintptr_t src, uintptr_t dst, size_t size);
  uintptr_t allocMem(size_t size);
  uintptr_t allocUtm(size_t size);
};

// Simulated memory reads/writes from calloc'ed memory
class SimulatedEnclaveMemory : public Memory {
 private:
  void* allocateAligned(size_t size, size_t alignment);

 public:
  SimulatedEnclaveMemory() {}
  ~SimulatedEnclaveMemory() {}
  void init(KeystoneDevice* dev, uintptr_t phys_addr, size_t min_pages);
  uintptr_t readMem(uintptr_t src, size_t size);
  void writeMem(uintptr_t src, uintptr_t dst, size_t size);
  uintptr_t allocMem(size_t size);
  uintptr_t allocUtm(size_t size);
};

}  // namespace Keystone

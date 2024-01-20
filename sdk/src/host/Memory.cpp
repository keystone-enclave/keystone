//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Memory.hpp"

#include <sys/stat.h>

#include "shared/keystone_user.h"

namespace Keystone {

Memory::Memory() {
  epmFreeList   = 0;
  startAddr     = 0;
}

void
Memory::startRuntimeMem() {
  runtimePhysAddr = getCurrentEPMAddress();
}

void
Memory::startEappMem() {
  eappPhysAddr = getCurrentEPMAddress();
}

void
Memory::startFreeMem() {
  freePhysAddr = getCurrentEPMAddress();
}

void 
Memory::incrementEPMFreeList() {
	epmFreeList += PAGE_SIZE;
}

uintptr_t
Memory::allocPages(size_t size) {
	uintptr_t addr = epmFreeList; 
	if (size % PAGE_SIZE > 0) {
    epmFreeList += (size / PAGE_SIZE + 1) * PAGE_SIZE;
  } else {
    epmFreeList += (size / PAGE_SIZE) * PAGE_SIZE;
  }
  return addr;
}

}  // namespace Keystone

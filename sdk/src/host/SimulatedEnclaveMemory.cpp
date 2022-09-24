//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Memory.hpp"

namespace Keystone {

void
SimulatedEnclaveMemory::init(
    KeystoneDevice* dev, uintptr_t phys_addr, size_t min_pages) {
  pDevice       = dev;
  epmSize       = PAGE_SIZE * min_pages;
  rootPageTable = allocMem(PAGE_SIZE * min_pages);
  startAddr     = rootPageTable;
  epmFreeList   = startAddr + PAGE_SIZE;
}

void*
SimulatedEnclaveMemory::allocateAligned(size_t size, size_t alignment) {
  const size_t mask   = alignment - 1;
  const uintptr_t mem = (uintptr_t)calloc(size + alignment, sizeof(char));
  return reinterpret_cast<void*>((mem + mask) & ~mask);
}

uintptr_t
SimulatedEnclaveMemory::allocMem(size_t size) {
  uintptr_t ret;
  ret = (uintptr_t)allocateAligned(size, PAGE_SIZE);
  return ret;
}

uintptr_t
SimulatedEnclaveMemory::allocUtm(size_t size) {
  utmFreeList   = allocMem(size);
  untrustedSize = size;
  utmPhysAddr   = utmFreeList;
  return utmFreeList;
}

uintptr_t
SimulatedEnclaveMemory::readMem(uintptr_t src, size_t size) {
  return src;
}

void
SimulatedEnclaveMemory::writeMem(uintptr_t src, uintptr_t dst, size_t size) {
  memcpy(reinterpret_cast<void*>(dst), reinterpret_cast<void*>(src), size);
}

}  // namespace Keystone

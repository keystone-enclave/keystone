//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Memory.hpp"

namespace Keystone {

void
PhysicalEnclaveMemory::init(
    KeystoneDevice* dev, uintptr_t phys_addr, size_t min_pages) {
  pDevice = dev;
  // TODO(dayeol): need to set actual EPM size
  epmSize       = PAGE_SIZE * min_pages;
  epmFreeList   = 0; 
  startAddr 		= phys_addr;
}

uintptr_t
PhysicalEnclaveMemory::allocUtm(size_t size) {
  uintptr_t ret = pDevice->initUTM(size);
  untrustedSize = size;
  utmPhysAddr   = ret;
  return ret;
}

// TODO: delete this 
/* Only used to allocate memory for root page table */
uintptr_t
PhysicalEnclaveMemory::allocMem(size_t size) {
  uintptr_t ret;

  assert(pDevice);

  ret = reinterpret_cast<uintptr_t>(pDevice->map(0, size));
  return ret;
}

// unused 
uintptr_t
PhysicalEnclaveMemory::readMem(uintptr_t src, size_t size) {
  uintptr_t ret;

  assert(pDevice);

  ret = reinterpret_cast<uintptr_t>(pDevice->map(src, size));
  return ret;
}

/* src: virtual address */
void
PhysicalEnclaveMemory::writeMem(uintptr_t src, uintptr_t offset, size_t size) {
  assert(pDevice);
  void* va_dst = pDevice->map(offset, size);
  memcpy(va_dst, reinterpret_cast<void*>(src), size);
}

}  // namespace Keystone

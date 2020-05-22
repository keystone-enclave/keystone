//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <sys/stat.h>
#include <keystone_user.h>
#include "Memory.h"

void SimulatedEnclaveMemory::init(KeystoneDevice* dev=nullptr, vaddr_t phys_addr=0)
{
  pDevice = dev;
}

void PhysicalEnclaveMemory::init(KeystoneDevice* dev, vaddr_t phys_addr)
{
  start_phys_addr = phys_addr;
  pDevice = dev;
}

void * SimulatedEnclaveMemory::allocateAligned(size_t size, size_t alignment)
{
  const size_t mask = alignment - 1;
  const uintptr_t mem = (uintptr_t) calloc(size + alignment, sizeof(char));
  return (void *) ((mem + mask) & ~mask);
}

vaddr_t PhysicalEnclaveMemory::AllocMem(size_t size){
  vaddr_t ret;

  assert(pDevice);

  ret = pDevice->map(0, PAGE_SIZE);
  return ret;
}

vaddr_t SimulatedEnclaveMemory::AllocMem(size_t size){
  vaddr_t ret;
  ret = (vaddr_t) allocateAligned(size, PAGE_SIZE);
  return ret;
}

vaddr_t PhysicalEnclaveMemory::ReadMem(vaddr_t src, size_t size){
  vaddr_t ret;

  assert(pDevice);

  ret = pDevice->map(src - start_phys_addr, size);
  return ret;
}

vaddr_t SimulatedEnclaveMemory::ReadMem(vaddr_t src, size_t size) {
  return src;
}

void PhysicalEnclaveMemory::WriteMem(vaddr_t src, vaddr_t dst, size_t size) {

  assert(pDevice);

  vaddr_t va_dst = pDevice->map(dst - start_phys_addr, size);
  memcpy((void *) va_dst, (void *) src, size);
}

void SimulatedEnclaveMemory::WriteMem(vaddr_t src, vaddr_t dst, size_t size) {
  memcpy((void*) dst, (void*) src, size);
}

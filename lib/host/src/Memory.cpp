//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/mman.h>
#include <keystone_user.h>
#include "Memory.h"

void SimulatedEnclaveMemory::init(int fd=0, vaddr_t phys_addr=0)
{
}

void PhysicalEnclaveMemory::init(int fd, vaddr_t phys_addr)
{
  start_phys_addr = phys_addr;
  keystone_fd = fd;
}

void * SimulatedEnclaveMemory::allocateAligned(size_t size, size_t alignment)
{
  const size_t mask = alignment - 1;
  const uintptr_t mem = (uintptr_t) calloc(size + alignment, sizeof(char));
  return (void *) ((mem + mask) & ~mask);
}

vaddr_t PhysicalEnclaveMemory::AllocMem(size_t size){
  vaddr_t ret;
  ret = (vaddr_t) mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, keystone_fd, 0);
  return ret;
}

vaddr_t SimulatedEnclaveMemory::AllocMem(size_t size){
  vaddr_t ret;
  ret = (vaddr_t) allocateAligned(size, PAGE_SIZE);
  return ret;
}

vaddr_t PhysicalEnclaveMemory::ReadMem(vaddr_t src, size_t size){
  vaddr_t ret;
  ret  = (vaddr_t) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, keystone_fd, src - start_phys_addr);
  return ret;
}

vaddr_t SimulatedEnclaveMemory::ReadMem(vaddr_t src, size_t size) {
  return src;
}

void PhysicalEnclaveMemory::WriteMem(vaddr_t src, vaddr_t dst, size_t size){
  vaddr_t va_dst = (vaddr_t) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, keystone_fd, dst - start_phys_addr);
  memcpy((void *) va_dst, (void *) src, size);
}

void SimulatedEnclaveMemory::WriteMem(vaddr_t src, vaddr_t dst, size_t size) {
  memcpy((void*) dst, (void*) src, size);
}

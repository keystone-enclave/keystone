//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <sys/stat.h>
#include <sys/mman.h>
#include <keystone_user.h>
#include "memory.h"

Memory::Memory() {
  start_phys_addr = 0;
  keystone_fd = 0;
  is_phys = 0;
}

Memory::~Memory() {

}

void Memory::init(int fd, vaddr_t phys_addr, bool _is_phys){
  keystone_fd = fd;
  start_phys_addr = phys_addr;
  is_phys = _is_phys;
}

void * allocate_aligned(size_t size, size_t alignment)
{
  const size_t mask = alignment - 1;
  const uintptr_t mem = (uintptr_t) calloc(size + alignment, sizeof(char));
  return (void *) ((mem + mask) & ~mask);
}

vaddr_t Memory::AllocMem(size_t size){

  vaddr_t ret;
  if(is_phys) {
    ret = (vaddr_t) mmap(NULL, PAGE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, keystone_fd, 0);
  }
  else{
    ret = (vaddr_t) allocate_aligned(size, PAGE_SIZE);
  }

  return ret;
}

vaddr_t Memory::ReadMem(vaddr_t src, size_t size){

  vaddr_t ret;
  if(is_phys) {
    ret  = (vaddr_t) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, keystone_fd, src - start_phys_addr);
  }
  else{
    ret = src;
  }
  return ret;
}


void Memory::WriteMem(vaddr_t src, vaddr_t dst, size_t size){

  if(is_phys) {
    vaddr_t va_dst = (vaddr_t) mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, keystone_fd, dst - start_phys_addr);
    memcpy((void *) va_dst, (void *) src, size);
  }
  else{
    memcpy((void *) dst, (void *) src, size);
  }
}


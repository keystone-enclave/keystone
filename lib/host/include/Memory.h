//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------

#include <stddef.h>
#include <cerrno>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <stdarg.h>
#include <assert.h>
#include "common.h"

class Memory
{
public:
  Memory() {};
  ~Memory() {};
  virtual void init(int fd, vaddr_t phys_addr) = 0;
  virtual vaddr_t ReadMem(vaddr_t src, size_t size) = 0;
  virtual void WriteMem(vaddr_t src, vaddr_t dst, size_t size) = 0;
  virtual vaddr_t AllocMem(size_t size) = 0;
};


class PhysicalEnclaveMemory : public Memory
{
private:
  int keystone_fd;
  vaddr_t start_phys_addr;
public:
  PhysicalEnclaveMemory() {};
  ~PhysicalEnclaveMemory() {};
  void init(int fd, vaddr_t phys_addr);
  vaddr_t ReadMem(vaddr_t src, size_t size);
  void WriteMem(vaddr_t src, vaddr_t dst, size_t size);
  vaddr_t AllocMem(size_t size);
};

// Simulated memory reads/writes from calloc'ed memory
class SimulatedEnclaveMemory : public Memory
{
private:
  void* allocateAligned(size_t size, size_t alignment);
public:
  SimulatedEnclaveMemory() {};
  ~SimulatedEnclaveMemory() {};
  void init(int fd, vaddr_t phys_addr);
  vaddr_t ReadMem(vaddr_t src, size_t size);
  void WriteMem(vaddr_t src, vaddr_t dst, size_t size);
  vaddr_t AllocMem(size_t size);
};

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

/*
 * These are used to make use of C type-checking..
 */
typedef struct {
  uintptr_t pte;
} pte;

#define pte_val(x) ((x).pte)

#define __pa(x) ((uintptr_t)(x))

#define __pte(x) ((pte){(x)})

// page table entry (PTE) fields
#define PTE_V 0x001     // Valid
#define PTE_R 0x002     // Read
#define PTE_W 0x004     // Write
#define PTE_X 0x008     // Execute
#define PTE_U 0x010     // User
#define PTE_G 0x020     // Global
#define PTE_A 0x040     // Accessed
#define PTE_D 0x080     // Dirty
#define PTE_SOFT 0x300  // Reserved for Software

#define PTE_PPN_SHIFT 10

#if __riscv_xlen == 32
#define VA_BITS 32
#define RISCV_PGLEVEL_BITS 10
#else  // __riscv_xlen == 64 or x86 test
#define VA_BITS 39
#define RISCV_PGLEVEL_BITS 9
#endif

#define RISCV_PGSHIFT 12
#define RISCV_PGSIZE (1 << RISCV_PGSHIFT)

#if __riscv_xlen == 64
#define RISCV_PGLEVEL_MASK 0x1ff
#define RISCV_PGTABLE_HIGHEST_BIT 0x100
#else
#define RISCV_PGLEVEL_MASK 0x3ff
#define RISCV_PGTABLE_HIGHEST_BIT 0x300
#endif

#define RISCV_PGLEVEL_TOP ((VA_BITS - RISCV_PGSHIFT) / RISCV_PGLEVEL_BITS)

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
  bool allocPage(uintptr_t eva, uintptr_t src, unsigned int mode);
  size_t epmAllocVspace(uintptr_t addr, size_t num_pages);

  // getters to be deprecated
  uintptr_t getStartAddr() { return startAddr; }
  uintptr_t getCurrentEPMAddress() { return epmFreeList; }
  uintptr_t getRootPageTable() { return rootPageTable; }

  int validateAndHashEpm(
      hash_ctx_t* hash_ctx, int level, pte* tb, uintptr_t vaddr, int contiguous,
      uintptr_t* runtime_max_seen, uintptr_t* user_max_seen);

  void startRuntimeMem();
  void startEappMem();
  void startFreeMem();

  uintptr_t getRuntimePhysAddr() { return runtimePhysAddr; }
  uintptr_t getEappPhysAddr() { return eappPhysAddr; }
  uintptr_t getFreePhysAddr() { return freePhysAddr; }

 protected:
  pte* __ept_walk_create(uintptr_t addr);
  pte* __ept_continue_walk_create(uintptr_t addr, pte* pte);
  pte* __ept_walk_internal(uintptr_t addr, int create);
  pte* __ept_walk(uintptr_t addr);
  uintptr_t epm_va_to_pa(uintptr_t addr);

  KeystoneDevice* pDevice;
  size_t epmSize;
  uintptr_t epmFreeList;
  uintptr_t utmFreeList;
  uintptr_t rootPageTable;
  uintptr_t startAddr;

  // for hash calculation
  uintptr_t runtimePhysAddr;
  uintptr_t eappPhysAddr;
  uintptr_t freePhysAddr;
  uintptr_t utmPhysAddr;
  uintptr_t untrustedPtr;
  uintptr_t untrustedSize;

 private:
  pte pte_create(uintptr_t, int);
  pte ptd_create(uintptr_t);
  uintptr_t pte_ppn(pte);
  uintptr_t ppn(uintptr_t);
  size_t pt_idx(uintptr_t, int);
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

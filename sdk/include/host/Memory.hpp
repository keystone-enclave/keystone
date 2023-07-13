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
#include <cstdint>
#include <iostream>
#include "./common.h"
#include "KeystoneDevice.hpp"
#include "hash_util.hpp"

namespace Keystone {

/*
 * These are used to make use of C type-checking..
 */
typedef struct {
  std::uintptr_t pte;
} pte;

#define pte_val(x) ((x).pte)

#define __pa(x) ((std::uintptr_t)(x))

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
  ~Memory() {}
  virtual void init(
      KeystoneDevice* dev, std::uintptr_t phys_addr, std::size_t min_pages)  = 0;
  virtual std::uintptr_t readMem(std::uintptr_t src, std::size_t size)            = 0;
  virtual void writeMem(std::uintptr_t src, std::uintptr_t dst, std::size_t size) = 0;
  virtual std::uintptr_t allocMem(std::size_t size)                          = 0;
  virtual std::uintptr_t allocUtm(std::size_t size)                          = 0;
  bool allocPage(std::uintptr_t eva, std::uintptr_t src, unsigned int mode);
  std::size_t epmAllocVspace(std::uintptr_t addr, std::size_t num_pages);

  // getters to be deprecated
  std::uintptr_t getStartAddr() const noexcept { return startAddr; }
  std::uintptr_t getCurrentEPMAddress() const noexcept { return epmFreeList; }
  std::uintptr_t getRootPageTable() const noexcept { return rootPageTable; }

  int validateAndHashEpm(
      hash_ctx_t* hash_ctx, int level, pte* tb, std::uintptr_t vaddr, int contiguous,
      std::uintptr_t* runtime_max_seen, std::uintptr_t* user_max_seen);

  void startRuntimeMem();
  void startEappMem();
  void startFreeMem();

  std::uintptr_t getRuntimePhysAddr() const noexcept { return runtimePhysAddr; }
  std::uintptr_t getEappPhysAddr() const noexcept { return eappPhysAddr; }
  std::uintptr_t getFreePhysAddr() const noexcept { return freePhysAddr; }

 protected:
  pte* __ept_walk_create(std::uintptr_t addr);
  pte* __ept_continue_walk_create(std::uintptr_t addr, pte* pte);
  pte* __ept_walk_internal(std::uintptr_t addr, int create);
  pte* __ept_walk(std::uintptr_t addr);
  std::uintptr_t epm_va_to_pa(std::uintptr_t addr);

  KeystoneDevice* pDevice;
  std::size_t epmSize;
  std::uintptr_t epmFreeList{ 0 };
  std::uintptr_t utmFreeList{ 0 };
  std::uintptr_t rootPageTable{ 0 };
  std::uintptr_t startAddr{ 0 };

  // for hash calculation
  std::uintptr_t runtimePhysAddr;
  std::uintptr_t eappPhysAddr;
  std::uintptr_t freePhysAddr;
  std::uintptr_t utmPhysAddr;
  std::uintptr_t untrustedPtr;
  std::uintptr_t untrustedSize;

 private:
  pte pte_create(std::uintptr_t, int);
  pte ptd_create(std::uintptr_t);
  std::uintptr_t pte_ppn(pte);
  std::uintptr_t ppn(std::uintptr_t);
  std::size_t pt_idx(std::uintptr_t, int);
};

class PhysicalEnclaveMemory : public Memory {
 public:
  PhysicalEnclaveMemory() {}
  ~PhysicalEnclaveMemory() {}
  void init(KeystoneDevice* dev, std::uintptr_t phys_addr, std::size_t min_pages);
  std::uintptr_t readMem(std::uintptr_t src, std::size_t size);
  void writeMem(std::uintptr_t src, std::uintptr_t dst, std::size_t size);
  std::uintptr_t allocMem(std::size_t size);
  std::uintptr_t allocUtm(std::size_t size);
};

// Simulated memory reads/writes from calloc'ed memory
class SimulatedEnclaveMemory : public Memory {
 private:
  void* allocateAligned(std::size_t size, std::size_t alignment);

 public:
  SimulatedEnclaveMemory() {}
  ~SimulatedEnclaveMemory() {}
  void init(KeystoneDevice* dev, std::uintptr_t phys_addr, std::size_t min_pages);
  std::uintptr_t readMem(std::uintptr_t src, std::size_t size);
  void writeMem(std::uintptr_t src, std::uintptr_t dst, std::size_t size);
  std::uintptr_t allocMem(std::size_t size);
  std::uintptr_t allocUtm(std::size_t size);
};

}  // namespace Keystone

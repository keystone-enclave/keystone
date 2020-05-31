//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Memory.hpp"
#include <keystone_user.h>
#include <sys/stat.h>

Memory::Memory() {
  epmFreeList = 0;
  utmFreeList = 0;
  rootPageTable = 0;
  startAddr = 0;
}

void
SimulatedEnclaveMemory::init(
    KeystoneDevice* dev, vaddr_t phys_addr, size_t min_pages) {
  pDevice = dev;
  epmFreeList = phys_addr + PAGE_SIZE;

  rootPageTable = AllocMem(PAGE_SIZE * min_pages);
  startAddr = rootPageTable;
}

void
PhysicalEnclaveMemory::init(
    KeystoneDevice* dev, vaddr_t phys_addr, size_t min_pages) {
  start_phys_addr = phys_addr;
  pDevice = dev;
  rootPageTable = AllocMem(PAGE_SIZE);
  epmFreeList = phys_addr + PAGE_SIZE;
  startAddr = phys_addr;
}

void*
SimulatedEnclaveMemory::allocateAligned(size_t size, size_t alignment) {
  const size_t mask = alignment - 1;
  const uintptr_t mem = (uintptr_t)calloc(size + alignment, sizeof(char));
  return reinterpret_cast<void*>((mem + mask) & ~mask);
}

vaddr_t
PhysicalEnclaveMemory::allocUTM(size_t size) {
  vaddr_t ret = pDevice->initUTM(size);
  utmFreeList = ret;
  return ret;
}

vaddr_t
PhysicalEnclaveMemory::AllocMem(size_t size) {
  vaddr_t ret;

  assert(pDevice);

  ret = reinterpret_cast<vaddr_t>(pDevice->map(0, PAGE_SIZE));
  return ret;
}

vaddr_t
SimulatedEnclaveMemory::AllocMem(size_t size) {
  vaddr_t ret;
  ret = (vaddr_t)allocateAligned(size, PAGE_SIZE);
  return ret;
}

vaddr_t
SimulatedEnclaveMemory::allocUTM(size_t size) {
  utmFreeList = AllocMem(size);
  return utmFreeList;
}

vaddr_t
PhysicalEnclaveMemory::ReadMem(vaddr_t src, size_t size) {
  vaddr_t ret;

  assert(pDevice);

  ret = reinterpret_cast<vaddr_t>(pDevice->map(src - start_phys_addr, size));
  return ret;
}

vaddr_t
SimulatedEnclaveMemory::ReadMem(vaddr_t src, size_t size) {
  return src;
}

void
PhysicalEnclaveMemory::WriteMem(vaddr_t src, vaddr_t dst, size_t size) {
  assert(pDevice);
  void* va_dst = pDevice->map(dst - start_phys_addr, size);
  memcpy(va_dst, reinterpret_cast<void*>(src), size);
}

void
SimulatedEnclaveMemory::WriteMem(vaddr_t src, vaddr_t dst, size_t size) {
  memcpy(reinterpret_cast<void*>(dst), reinterpret_cast<void*>(src), size);
}

bool
Memory::allocPage(vaddr_t va, vaddr_t src, unsigned int mode) {
  vaddr_t page_addr;
  vaddr_t* pFreeList = (mode == UTM_FULL ? &utmFreeList : &epmFreeList);

  pte_t* pte = __ept_walk_create(va);

  /* if the page has been already allocated, return the page */
  if (pte_val(*pte) & PTE_V) {
    return true;
  }

  /* otherwise, allocate one from EPM freelist */
  page_addr = *pFreeList >> PAGE_BITS;
  *pFreeList += PAGE_SIZE;

  switch (mode) {
    case USER_NOEXEC: {
      *pte =
          pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_U | PTE_V);
      break;
    }
    case RT_NOEXEC: {
      *pte = pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_V);
      break;
    }
    case RT_FULL: {
      *pte =
          pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V);
      WriteMem(src, (vaddr_t)page_addr << PAGE_BITS, PAGE_SIZE);
      break;
    }
    case USER_FULL: {
      *pte = pte_create(
          page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_U | PTE_V);
      WriteMem(src, (vaddr_t)page_addr << PAGE_BITS, PAGE_SIZE);
      break;
    }
    case UTM_FULL: {
      *pte = pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_V);
      WriteMem(src, (vaddr_t)page_addr << PAGE_BITS, PAGE_SIZE);
      break;
    }
    default: {
      PERROR("failed to add page - mode is invalid");
      return false;
    }
  }

  return true;
}

pte_t*
Memory::__ept_continue_walk_create(vaddr_t addr, pte_t* pte) {
  uint64_t free_ppn = ppn(epmFreeList);
  *pte = ptd_create(free_ppn);
  epmFreeList += PAGE_SIZE;
  return __ept_walk_create(addr);
}

pte_t*
Memory::__ept_walk_internal(vaddr_t addr, int create) {
  pte_t* t = reinterpret_cast<pte_t*>(rootPageTable);

  int i;
  for (i = (VA_BITS - RISCV_PGSHIFT) / RISCV_PGLEVEL_BITS - 1; i > 0; i--) {
    size_t idx = pt_idx(addr, i);
    if (!(pte_val(t[idx]) & PTE_V)) {
      return create ? __ept_continue_walk_create(addr, &t[idx]) : 0;
    }

    t = reinterpret_cast<pte_t*>(
        ReadMem(
            reinterpret_cast<vaddr_t>(pte_ppn(t[idx]) << RISCV_PGSHIFT),
            PAGE_SIZE));
  }
  return &t[pt_idx(addr, 0)];
}

pte_t*
Memory::__ept_walk_create(vaddr_t addr) {
  return __ept_walk_internal(addr, 1);
}

pte_t*
Memory::__ept_walk(vaddr_t addr) {
  return __ept_walk_internal(addr, 0);
}

vaddr_t
Memory::epm_va_to_pa(vaddr_t addr) {
  pte_t* pte = __ept_walk(addr);
  if (pte)
    return pte_ppn(*pte) << RISCV_PGSHIFT;
  else
    return 0;
}

/* This function pre-allocates the required page tables so that
 * the virtual addresses are linearly mapped to the physical memory */
size_t
Memory::epm_alloc_vspace(vaddr_t addr, size_t num_pages) {
  size_t count;

  for (count = 0; count < num_pages; count++, addr += PAGE_SIZE) {
    pte_t* pte = __ept_walk_create(addr);
    if (!pte) break;
  }

  return count;
}

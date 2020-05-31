//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Memory.hpp"
#include <keystone_user.h>
#include <sys/stat.h>

Memory::Memory() {
  epmFreeList   = 0;
  utmFreeList   = 0;
  rootPageTable = 0;
  startAddr     = 0;
}

void Memory::startRuntimeMem() {
  runtimePhysAddr = getCurrentEPMAddress();
}

void Memory::startEappMem() {
  eappPhysAddr = getCurrentEPMAddress();
}

void Memory::startFreeMem() {
  freePhysAddr = getCurrentEPMAddress();
}

void
SimulatedEnclaveMemory::init(
    KeystoneDevice* dev, vaddr_t phys_addr, size_t min_pages) {
  pDevice     = dev;
  epmSize = PAGE_SIZE * min_pages;
  rootPageTable = AllocMem(PAGE_SIZE * min_pages);
  startAddr     = rootPageTable;
  epmFreeList = startAddr + PAGE_SIZE;
}

void
PhysicalEnclaveMemory::init(
    KeystoneDevice* dev, vaddr_t phys_addr, size_t min_pages) {
  pDevice         = dev;
  // TODO: need to set actual EPM size
  epmSize = PAGE_SIZE * min_pages;
  rootPageTable   = AllocMem(PAGE_SIZE);
  epmFreeList     = phys_addr + PAGE_SIZE;
  startAddr       = phys_addr;
}

void*
SimulatedEnclaveMemory::allocateAligned(size_t size, size_t alignment) {
  const size_t mask   = alignment - 1;
  const uintptr_t mem = (uintptr_t)calloc(size + alignment, sizeof(char));
  return reinterpret_cast<void*>((mem + mask) & ~mask);
}

vaddr_t
PhysicalEnclaveMemory::allocUTM(size_t size) {
  vaddr_t ret = pDevice->initUTM(size);
  utmFreeList = ret;
  untrustedSize = size;
  utmPhysAddr = ret;
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
  ret = (vaddr_t) allocateAligned(size, PAGE_SIZE);
  return ret;
}

vaddr_t
SimulatedEnclaveMemory::allocUTM(size_t size) {
  utmFreeList = AllocMem(size);
  untrustedSize = size;
  utmPhysAddr = utmFreeList;
  return utmFreeList;
}

vaddr_t
PhysicalEnclaveMemory::ReadMem(vaddr_t src, size_t size) {
  vaddr_t ret;

  assert(pDevice);

  ret = reinterpret_cast<vaddr_t>(pDevice->map(src - startAddr, size));
  return ret;
}

vaddr_t
SimulatedEnclaveMemory::ReadMem(vaddr_t src, size_t size) {
  return src;
}

void
PhysicalEnclaveMemory::WriteMem(vaddr_t src, vaddr_t dst, size_t size) {
  assert(pDevice);
  void* va_dst = pDevice->map(dst - startAddr, size);
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
  *pte              = ptd_create(free_ppn);
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

/* This will walk the entire vaddr space in the enclave, validating
   linear at-most-once paddr mappings, and then hashing valid pages */
int Memory::validate_and_hash_epm(hash_ctx_t* hash_ctx, int level,
                          pte_t* tb, uintptr_t vaddr, int contiguous,
                          uintptr_t* runtime_max_seen,
                          uintptr_t* user_max_seen)
{
  pte_t* walk;
  int i;

  /* iterate over PTEs */
  for (walk=tb, i=0; walk < tb + (RISCV_PGSIZE/sizeof(pte_t)); walk += 1,i++)
  {
    if (pte_val(*walk) == 0) {
      contiguous = 0;
      continue;
    }
    uintptr_t vpn;
    uintptr_t phys_addr = (pte_val(*walk) >> PTE_PPN_SHIFT) << RISCV_PGSHIFT;
    /* Check for blatently invalid mappings */
    int map_in_epm = (phys_addr >= startAddr &&
                      phys_addr < startAddr + epmSize);
    int map_in_utm = (phys_addr >= utmPhysAddr &&
                      phys_addr < utmPhysAddr + untrustedSize);


    /* EPM may map anything, UTM may not map pgtables */
    if(!map_in_epm && (!map_in_utm || level != 1)){
      printf("1\n");
      goto fatal_bail;
    }

    /* propagate the highest bit of the VA */
    if ( level == RISCV_PGLEVEL_TOP && i & RISCV_PGTABLE_HIGHEST_BIT )
      vpn = ((-1UL << RISCV_PGLEVEL_BITS) | (i & RISCV_PGLEVEL_MASK));
    else
      vpn = ((vaddr << RISCV_PGLEVEL_BITS) | (i & RISCV_PGLEVEL_MASK));

    uintptr_t va_start = vpn << RISCV_PGSHIFT;

    /* include the first virtual address of a contiguous range */
    if (level == 1 && !contiguous)
    {

      hash_extend(hash_ctx, &va_start, sizeof(uintptr_t));
//      printf("user VA hashed: 0x%lx\n", va_start);
      contiguous = 1;
    }

    if (level == 1)
    {

      /*
       * This is where we enforce the at-most-one-mapping property.
       * To make our lives easier, we also require a 'linear' mapping
       * (for each of the user and runtime spaces independently).
       *
       * That is: Given V1->P1 and V2->P2:
       *
       * V1 < V2  ==> P1 < P2  (Only for within a given space)
       *
       * V1 != V2 ==> P1 != P2
       *
       * We also validate that all utm vaddrs -> utm paddrs
       */
      int in_runtime = ((phys_addr >= runtimePhysAddr) &&
                        (phys_addr < eappPhysAddr));
      int in_user = ((phys_addr >= eappPhysAddr) &&
                     (phys_addr < freePhysAddr));

      /* Validate U bit */
      if(in_user && !(pte_val(*walk) & PTE_U)){
        goto fatal_bail;
      }

      /* If the vaddr is in UTM, the paddr must be in UTM */
      if(va_start >= utmPhysAddr &&
         va_start < (utmPhysAddr + untrustedSize) &&
         !map_in_utm){
        goto fatal_bail;
      }

      /* Do linear mapping validation */
      if(in_runtime){
        if(phys_addr <= *runtime_max_seen){
          goto fatal_bail;
        }
        else{
          *runtime_max_seen = phys_addr;
        }
      }
      else if(in_user){
        if(phys_addr <= *user_max_seen){
          goto fatal_bail;
        }
        else{
          *user_max_seen = phys_addr;
        }
      }
      else if(map_in_utm){
        // we checked this above, its OK
      }
      else{
//        printf("BAD GENERIC MAP %x %x %x\n", in_runtime, in_user, map_in_utm);
        goto fatal_bail;
      }

      /* Page is valid, add it to the hash */

      /* if PTE is leaf, extend hash for the page */
      hash_extend_page(hash_ctx, (void*)phys_addr);
//      printf("user PAGE hashed: 0x%lx (pa: 0x%lx)\n", vpn << RISCV_PGSHIFT, phys_addr);
    }
    else
    {
      /* otherwise, recurse on a lower level */
      contiguous = validate_and_hash_epm(hash_ctx,
                                         level - 1,
                                         (pte_t*) phys_addr,
                                         vpn,
                                         contiguous,
                                         runtime_max_seen,
                                         user_max_seen
                                         );
      if(contiguous == -1){
        printf("BAD MAP: %p->%p epm %u %p uer %u %p\n",
               (void *) va_start,
               (void *) phys_addr,
//                in_runtime,
                0,
               (void *) runtimePhysAddr,
//                in_user,
                0,
               (void *) eappPhysAddr);
        goto fatal_bail;
      }
    }
  }

  return contiguous;

  fatal_bail:
  return -1;
}


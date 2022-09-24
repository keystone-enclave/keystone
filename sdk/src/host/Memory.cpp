//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "Memory.hpp"
#include <keystone_user.h>
#include <sys/stat.h>

namespace Keystone {

Memory::Memory() {
  epmFreeList   = 0;
  utmFreeList   = 0;
  rootPageTable = 0;
  startAddr     = 0;
}

void
Memory::startRuntimeMem() {
  runtimePhysAddr = getCurrentEPMAddress();
}

void
Memory::startEappMem() {
  eappPhysAddr = getCurrentEPMAddress();
}

void
Memory::startFreeMem() {
  freePhysAddr = getCurrentEPMAddress();
}

inline pte
Memory::pte_create(uintptr_t ppn, int type) {
  return __pte((ppn << PTE_PPN_SHIFT) | PTE_V | type);
}

inline pte
Memory::ptd_create(uintptr_t ppn) {
  return pte_create(ppn, PTE_V);
}

uintptr_t
Memory::pte_ppn(pte pte) {
  return pte_val(pte) >> PTE_PPN_SHIFT;
}

uintptr_t
Memory::ppn(uintptr_t addr) {
  return __pa(addr) >> RISCV_PGSHIFT;
}

size_t
Memory::pt_idx(uintptr_t addr, int level) {
  size_t idx = addr >> (RISCV_PGLEVEL_BITS * level + RISCV_PGSHIFT);
  return idx & ((1 << RISCV_PGLEVEL_BITS) - 1);
}

bool
Memory::allocPage(uintptr_t va, uintptr_t src, unsigned int mode) {
  uintptr_t page_addr;
  uintptr_t* pFreeList = (mode == UTM_FULL ? &utmFreeList : &epmFreeList);

  pte* pte = __ept_walk_create(va);

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
      writeMem(src, (uintptr_t)page_addr << PAGE_BITS, PAGE_SIZE);
      break;
    }
    case USER_FULL: {
      *pte = pte_create(
          page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_U | PTE_V);
      writeMem(src, (uintptr_t)page_addr << PAGE_BITS, PAGE_SIZE);
      break;
    }
    case UTM_FULL: {
      assert(!src);
      *pte = pte_create(page_addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_V);
      break;
    }
    default: {
      PERROR("failed to add page - mode is invalid");
      return false;
    }
  }

  return true;
}

pte*
Memory::__ept_continue_walk_create(uintptr_t addr, pte* pte) {
  uint64_t free_ppn = ppn(epmFreeList);
  *pte              = ptd_create(free_ppn);
  epmFreeList += PAGE_SIZE;
  return __ept_walk_create(addr);
}

pte*
Memory::__ept_walk_internal(uintptr_t addr, int create) {
  pte* t = reinterpret_cast<pte*>(rootPageTable);

  int i;
  for (i = (VA_BITS - RISCV_PGSHIFT) / RISCV_PGLEVEL_BITS - 1; i > 0; i--) {
    size_t idx = pt_idx(addr, i);
    if (!(pte_val(t[idx]) & PTE_V)) {
      return create ? __ept_continue_walk_create(addr, &t[idx]) : 0;
    }

    t = reinterpret_cast<pte*>(readMem(
        reinterpret_cast<uintptr_t>(pte_ppn(t[idx]) << RISCV_PGSHIFT),
        PAGE_SIZE));
  }
  return &t[pt_idx(addr, 0)];
}

pte*
Memory::__ept_walk_create(uintptr_t addr) {
  return __ept_walk_internal(addr, 1);
}

pte*
Memory::__ept_walk(uintptr_t addr) {
  return __ept_walk_internal(addr, 0);
}

uintptr_t
Memory::epm_va_to_pa(uintptr_t addr) {
  pte* pte = __ept_walk(addr);
  if (pte)
    return pte_ppn(*pte) << RISCV_PGSHIFT;
  else
    return 0;
}

/* This function pre-allocates the required page tables so that
 * the virtual addresses are linearly mapped to the physical memory */
size_t
Memory::epmAllocVspace(uintptr_t addr, size_t num_pages) {
  size_t count;

  for (count = 0; count < num_pages; count++, addr += PAGE_SIZE) {
    pte* pte = __ept_walk_create(addr);
    if (!pte) break;
  }

  return count;
}

/* This will walk the entire vaddr space in the enclave, validating
   linear at-most-once paddr mappings, and then hashing valid pages */
int
Memory::validateAndHashEpm(
    hash_ctx_t* hash_ctx, int level, pte* tb, uintptr_t vaddr, int contiguous,
    uintptr_t* runtime_max_seen, uintptr_t* user_max_seen) {
  pte* walk;
  int i;

  /* iterate over PTEs */
  for (walk = tb, i = 0; walk < tb + (RISCV_PGSIZE / sizeof(pte));
       walk += 1, i++) {
    if (pte_val(*walk) == 0) {
      contiguous = 0;
      continue;
    }
    uintptr_t vpn;
    uintptr_t phys_addr = (pte_val(*walk) >> PTE_PPN_SHIFT) << RISCV_PGSHIFT;
    /* Check for blatently invalid mappings */
    int map_in_epm =
        (phys_addr >= startAddr && phys_addr < startAddr + epmSize);
    int map_in_utm =
        (phys_addr >= utmPhysAddr && phys_addr < utmPhysAddr + untrustedSize);

    /* EPM may map anything, UTM may not map pgtables */
    if (!map_in_epm && (!map_in_utm || level != 1)) {
      printf("1\n");
      goto fatal_bail;
    }

    /* propagate the highest bit of the VA */
    if (level == RISCV_PGLEVEL_TOP && i & RISCV_PGTABLE_HIGHEST_BIT)
      vpn = ((-1UL << RISCV_PGLEVEL_BITS) | (i & RISCV_PGLEVEL_MASK));
    else
      vpn = ((vaddr << RISCV_PGLEVEL_BITS) | (i & RISCV_PGLEVEL_MASK));

    uintptr_t va_start = vpn << RISCV_PGSHIFT;

    /* include the first virtual address of a contiguous range */
    if (level == 1 && !contiguous) {
      hash_extend(hash_ctx, &va_start, sizeof(uintptr_t));
      //      printf("user VA hashed: 0x%lx\n", va_start);
      contiguous = 1;
    }

    if (level == 1) {
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
      int in_runtime =
          ((phys_addr >= runtimePhysAddr) && (phys_addr < eappPhysAddr));
      int in_user = ((phys_addr >= eappPhysAddr) && (phys_addr < freePhysAddr));

      /* Validate U bit */
      if (in_user && !(pte_val(*walk) & PTE_U)) {
        goto fatal_bail;
      }

      /* If the vaddr is in UTM, the paddr must be in UTM */
      if (va_start >= utmPhysAddr && va_start < (utmPhysAddr + untrustedSize) &&
          !map_in_utm) {
        goto fatal_bail;
      }

      /* Do linear mapping validation */
      if (in_runtime) {
        if (phys_addr <= *runtime_max_seen) {
          goto fatal_bail;
        } else {
          *runtime_max_seen = phys_addr;
        }
      } else if (in_user) {
        if (phys_addr <= *user_max_seen) {
          goto fatal_bail;
        } else {
          *user_max_seen = phys_addr;
        }
      } else if (map_in_utm) {
        // we checked this above, its OK
      } else {
        //        printf("BAD GENERIC MAP %x %x %x\n", in_runtime, in_user,
        //        map_in_utm);
        goto fatal_bail;
      }

      /* Page is valid, add it to the hash */

      /* if PTE is leaf, extend hash for the page */
      hash_extend_page(hash_ctx, reinterpret_cast<void*>(phys_addr));
      //      printf("user PAGE hashed: 0x%lx (pa: 0x%lx)\n", vpn <<
      //      RISCV_PGSHIFT, phys_addr);
    } else {
      /* otherwise, recurse on a lower level */
      contiguous = validateAndHashEpm(
          hash_ctx, level - 1, reinterpret_cast<pte*>(phys_addr), vpn,
          contiguous, runtime_max_seen, user_max_seen);
      if (contiguous == -1) {
        printf(
            "BAD MAP: %p->%p epm %u %p uer %u %p\n",
            reinterpret_cast<void*>(va_start),
            reinterpret_cast<void*>(phys_addr),
            //                in_runtime,
            0, reinterpret_cast<void*>(runtimePhysAddr),
            //                in_user,
            0, reinterpret_cast<void*>(eappPhysAddr));
        goto fatal_bail;
      }
    }
  }

  return contiguous;

fatal_bail:
  return -1;
}

}  // namespace Keystone

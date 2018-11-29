//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "page.h"

int init_encl_pgtable(int level, pte_t* tb, uintptr_t base, uintptr_t size, uintptr_t utbase, uintptr_t utsize){
  pte_t* walk;
  int ret = 0;
  int i=0;
  
  for (walk=tb; walk < tb + (RISCV_PGSIZE/sizeof(pte_t)) ; walk += 1)
  {
    if(*walk == 0)
      continue;

    pte_t pte = *walk;
    uintptr_t phys_addr = (pte >> PTE_PPN_SHIFT) << RISCV_PGSHIFT;
    if(!((phys_addr >= base && phys_addr < base + size) ||
        (phys_addr >= utbase && phys_addr< utbase + utsize)))
    {
      *walk = 0;
      continue;
    }
    if(level > 1)
    {
      ret |= init_encl_pgtable(level - 1, (pte_t*) phys_addr, base, size, utbase, utsize);
    }
  }
  return ret;
}

static uintptr_t pte_ppn(pte_t pte)
{
  return pte >> PTE_PPN_SHIFT;
}

static size_t pt_idx(uintptr_t addr, int level)
{
  size_t idx = addr >> (RISCV_PGLEVEL_BITS*level + RISCV_PGSHIFT);
  return idx & ((1 << RISCV_PGLEVEL_BITS) - 1);
}

static uintptr_t __ept_walk_internal(pte_t* root_page_table, uintptr_t addr)
{
  pte_t* t = root_page_table;
  int i;
  for (i = (VA_BITS - RISCV_PGSHIFT) / RISCV_PGLEVEL_BITS - 1; i > 0; i--) {
    size_t idx = pt_idx(addr, i);
    if (!(t[idx] & PTE_V))
      return 0;
    t = (pte_t*) (pte_ppn(t[idx]) << RISCV_PGSHIFT);
  }
  return t[pt_idx(addr, 0)];
}

static uintptr_t __ept_walk(pte_t* root_page_table, uintptr_t addr)
{
  return __ept_walk_internal(root_page_table, addr);
}

uintptr_t get_phys_addr(uintptr_t addr)
{
  pte_t pte;
  uintptr_t physaddr;
  pte =  __ept_walk((pte_t*) (read_csr(satp) << RISCV_PGSHIFT), addr);
  
  physaddr = pte_ppn(pte) << RISCV_PGSHIFT;
  physaddr |= addr & (RISCV_PGSIZE - 1);
  return physaddr;
}

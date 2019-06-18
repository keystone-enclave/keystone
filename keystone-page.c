//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "riscv64.h"
#include <linux/kernel.h>
#include "keystone.h"
#include <linux/dma-mapping.h>

/* Destroy all memory associated with an EPM */
int epm_destroy(struct epm* epm) {

  if(!epm->ptr || !epm->size)
    return 0;

  /* free the EPM hold by the enclave */
  if (epm->is_cma) {
    dma_free_coherent(keystone_dev.this_device,
        epm->size,
        (void*) epm->ptr,
        epm->pa);
  } else {
    free_pages(epm->ptr, epm->order);
  }

  return 0;
}

/* Create an EPM and initialize the free list */
int epm_init(struct epm* epm, unsigned int min_pages)
{
  pte_t* t;

  vaddr_t epm_vaddr = 0;
  unsigned long order = 0;
  unsigned long count = min_pages;
  phys_addr_t device_phys_addr = 0;

  /* try to allocate contiguous memory */
  epm->is_cma = 0;
  order = ilog2(min_pages - 1) + 1;
  count = 0x1 << order;

  /* prevent kernel from complaining about an invalid argument */
  if (order <= MAX_ORDER)
    epm_vaddr = (vaddr_t) __get_free_pages(GFP_HIGHUSER, order);

#ifdef CONFIG_CMA
  /* If buddy allocator fails, we fall back to the CMA */
  if (!epm_vaddr) {
    epm->is_cma = 1;
    count = min_pages;

    epm_vaddr = (vaddr_t) dma_alloc_coherent(keystone_dev.this_device,
      count << PAGE_SHIFT,
      &device_phys_addr,
      GFP_KERNEL);

    if(!device_phys_addr)
      epm_vaddr = 0;
  }
#endif

  if(!epm_vaddr) {
    keystone_err("failed to allocate %lu page(s)\n", count);
    return -ENOMEM;
  }

  /* zero out */
  memset((void*)epm_vaddr, 0, PAGE_SIZE*count);

  epm->root_page_table = epm_vaddr;
  epm->pa = __pa(epm_vaddr);
  epm->order = order;
  epm->size = count << PAGE_SHIFT;
  epm->ptr = epm_vaddr;

  return 0;
}

int utm_destroy(struct utm* utm){

  if(utm->ptr != NULL){
    free_pages((vaddr_t)utm->ptr, utm->order);
  }

  return 0;
}

int utm_init(struct utm* utm, size_t untrusted_size)
{
  unsigned long req_pages = 0;
  unsigned long order = 0;
  unsigned long count;
  req_pages += PAGE_UP(untrusted_size)/PAGE_SIZE;
  order = ilog2(req_pages - 1) + 1;
  count = 0x1 << order;

  utm->order = order;

  /* Currently, UTM does not utilize CMA.
   * It is always allocated from the buddy allocator */
  utm->ptr = (void*) __get_free_pages(GFP_HIGHUSER, order);
  if (!utm->ptr) {
    return -ENOMEM;
  }

  utm->size = count * PAGE_SIZE;
  if (utm->size != untrusted_size) {
    /* Instead of failing, we just warn that the user has to fix the parameter. */
    keystone_warn("shared buffer size is not multiple of PAGE_SIZE\n");
  }

  return 0;
}

static paddr_t pte_ppn(pte_t pte)
{
  return pte_val(pte) >> PTE_PPN_SHIFT;
}

static paddr_t ppn(vaddr_t addr)
{
  return __pa(addr) >> RISCV_PGSHIFT;
}

static size_t pt_idx(vaddr_t addr, int level)
{
  size_t idx = addr >> (RISCV_PGLEVEL_BITS*level + RISCV_PGSHIFT);
  return idx & ((1 << RISCV_PGLEVEL_BITS) - 1);
}

static pte_t* __ept_walk_internal(struct list_head* pg_list, pte_t* root_page_table, vaddr_t addr, int create)
{
  pte_t* t = root_page_table;
  //pr_info("  page walk:\n");
  int i;
  for (i = (VA_BITS - RISCV_PGSHIFT) / RISCV_PGLEVEL_BITS - 1; i > 0; i--) {
    size_t idx = pt_idx(addr, i);
    /*Since page management is done on the user side, the PTE must be valid
     * Otherwise the VA is invalid
     * */
    if (unlikely(!(pte_val(t[idx]) & PTE_V)))
      return -1;
    t = (pte_t*) __va(pte_ppn(t[idx]) << RISCV_PGSHIFT);
  }
  return &t[pt_idx(addr, 0)];
}

static pte_t* __ept_walk(struct list_head* pg_list, pte_t* root_page_table, vaddr_t addr)
{
  return __ept_walk_internal(pg_list, root_page_table, addr, 0);
}

paddr_t epm_va_to_pa(struct epm* epm, vaddr_t addr)
{
  pte_t* pte = __ept_walk(NULL, epm->root_page_table,addr);
  if(pte)
    return pte_ppn(*pte) << RISCV_PGSHIFT;
  else
    return 0;
}

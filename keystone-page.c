//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "riscv64.h"
#include <linux/kernel.h>
#include "keystone.h"

void init_free_pages(struct list_head* pg_list, vaddr_t base, unsigned int count)
{
  unsigned int i;
  vaddr_t cur;
  cur = base;
  for(i=0; i<count; i++)
  {
    put_free_page(pg_list, cur);
    cur += RISCV_PGSIZE;
  }
  return;
}

vaddr_t get_free_page(struct list_head* pg_list)
{
  struct free_page_t* page;
  vaddr_t addr;

  if(list_empty(pg_list))
    return 0;

  page = list_first_entry(pg_list, struct free_page_t, freelist);
  addr = page->vaddr;
  list_del(&page->freelist);
  kfree(page);
  
  return addr;
}

void put_free_page(struct list_head* pg_list, vaddr_t page_addr)
{
  struct free_page_t* page = kmalloc(sizeof(struct free_page_t),GFP_KERNEL);
  page->vaddr = page_addr;
  list_add_tail(&page->freelist, pg_list);
  return;
}

int epm_destroy(epm_t* epm){

  /* Clean anything in the free list */
  epm_clean_free_list(epm);

  if(epm->base != 0){
    free_pages(epm->base, epm->order);
  }
  
  return 0;
}

void epm_init(epm_t* epm, vaddr_t base, unsigned int count)
{
  pte_t* t;
  
  init_free_pages(&epm->epm_free_list, base, count); 
  epm->base = base;
  epm->total = count * PAGE_SIZE; 

  t = (pte_t*) get_free_page(&epm->epm_free_list);
  epm->root_page_table = t;
  
  return;
}

int epm_clean_free_list(epm_t* epm)
{
  struct free_page_t* page;
  struct list_head* pg_list;
  pg_list = &epm->epm_free_list;
  while (!list_empty(&epm->epm_free_list))
  {
    page = list_first_entry(pg_list, struct free_page_t, freelist);
    list_del(&page->freelist);
    kfree(page);
  }
  return 0;
}

int utm_destroy(utm_t* utm){

  /* Clean anything in the free list */
  utm_clean_free_list(utm);

  if(utm->ptr != NULL){
    free_pages((vaddr_t)utm->ptr, utm->order);
  }
  
  return 0;
}

int utm_clean_free_list(utm_t* utm)
{
  struct free_page_t* page;
  struct list_head* pg_list;
  pg_list = &utm->utm_free_list;
  while (!list_empty(&utm->utm_free_list))
  {
    page = list_first_entry(pg_list, struct free_page_t, freelist);
    list_del(&page->freelist);
    kfree(page);
  }
  return 0;
}

int utm_init(utm_t* utm, size_t untrusted_size)
{
  unsigned long req_pages = 0;
  unsigned long order = 0;
  unsigned long count;
  req_pages += PAGE_UP(untrusted_size)/PAGE_SIZE;
  order = ilog2(req_pages - 1) + 1;
  count = 0x1 << order;

  utm->order = order;
  
  utm->ptr = (void*) __get_free_pages(GFP_HIGHUSER, order);
  if (!utm->ptr) {
    return -ENOMEM;
  }

  utm->size = count * PAGE_SIZE;
  if (utm->size != untrusted_size) {
    keystone_warn("shared buffer size is not multiple of PAGE_SIZE\n");
  }

  INIT_LIST_HEAD(&utm->utm_free_list);
  init_free_pages(&utm->utm_free_list, (vaddr_t)utm->ptr, utm->size/PAGE_SIZE);
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

static pte_t* __ept_walk_create(struct list_head* pg_list, pte_t* root_page_table, vaddr_t addr);

static pte_t* __ept_continue_walk_create(struct list_head* pg_list, pte_t* root_page_table, vaddr_t addr, pte_t* pte)
{
  unsigned long free_ppn = ppn(get_free_page(pg_list));
  *pte = ptd_create(free_ppn);
  //pr_info("ptd_create: ppn = %u, pte = 0x%lx\n", free_ppn,  *pte);
  return __ept_walk_create(pg_list, root_page_table, addr);
}

static pte_t* __ept_walk_internal(struct list_head* pg_list, pte_t* root_page_table, vaddr_t addr, int create)
{
  pte_t* t = root_page_table;
  //pr_info("  page walk:\n");
  int i;
  for (i = (VA_BITS - RISCV_PGSHIFT) / RISCV_PGLEVEL_BITS - 1; i > 0; i--) {
    size_t idx = pt_idx(addr, i);
    //pr_info("    level %d: pt_idx %d (%x)\n", i, idx, idx);
    if (unlikely(!(pte_val(t[idx]) & PTE_V)))
      return create ? __ept_continue_walk_create(pg_list, root_page_table, addr, &t[idx]) : 0;
    t = (pte_t*) __va(pte_ppn(t[idx]) << RISCV_PGSHIFT);
  }
  return &t[pt_idx(addr, 0)];
}
/*
static pte_t* __ept_walk(struct list_head* pg_list, pte_t* root_page_table, vaddr_t addr)
{
  return __ept_walk_internal(pg_list, root_page_table, addr, 0);
}
*/

static pte_t* __ept_walk_create(struct list_head* pg_list, pte_t* root_page_table, vaddr_t addr)
{
  return __ept_walk_internal(pg_list, root_page_table, addr, 1);
}

/*
static int __ept_va_avail(epm_t* epm, vaddr_t vaddr)
{
  pte_t* pte = __ept_walk(epm, vaddr);
  return pte == 0 || pte_val(*pte) == 0;
}
*/

vaddr_t utm_alloc_page(utm_t* utm, epm_t* epm, vaddr_t addr, unsigned long flags)
{
  pte_t* pte = __ept_walk_create(&epm->epm_free_list, epm->root_page_table, addr);
  vaddr_t page_addr = get_free_page(&utm->utm_free_list);
  *pte = pte_create(ppn(page_addr), flags | PTE_V);
  return page_addr;
}

vaddr_t epm_alloc_page(epm_t* epm, vaddr_t addr, unsigned long flags)
{
  pte_t* pte = __ept_walk_create(&epm->epm_free_list, epm->root_page_table, addr);
  vaddr_t page_addr = get_free_page(&epm->epm_free_list);
  *pte = pte_create(ppn(page_addr), flags | PTE_V);
  return page_addr;
}

vaddr_t epm_alloc_rt_page_noexec(epm_t* epm, vaddr_t addr)
{
  return epm_alloc_page(epm, addr, PTE_D | PTE_A | PTE_R | PTE_W);
}

vaddr_t epm_alloc_rt_page(epm_t* epm, vaddr_t addr)
{
  return epm_alloc_page(epm, addr, PTE_A | PTE_R | PTE_W | PTE_X);
}

vaddr_t epm_alloc_user_page_noexec(epm_t* epm, vaddr_t addr)
{
  return epm_alloc_page(epm, addr, PTE_D | PTE_A | PTE_R | PTE_W | PTE_U);
}

vaddr_t epm_alloc_user_page(epm_t* epm, vaddr_t addr)
{
  return epm_alloc_page(epm, addr, PTE_A | PTE_R | PTE_X | PTE_W | PTE_U);
}

void epm_free_page(epm_t* epm, vaddr_t addr)
{
  /* TODO: Must Implement Quickly */
}

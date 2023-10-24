#include "mm.h"
#include "vm.h"
#include "mem.h"
#include "common.h"

uintptr_t freeList; 
uintptr_t epmBase; 
size_t epmSize;

/* Page table utilities */
static pte*
__walk_create(pte* root, uintptr_t addr);

static uintptr_t get_new_page() {
  uintptr_t new_page = freeList; 
  if (new_page > epmBase + epmSize) {
    return -1;
  } 

  freeList += RISCV_PAGE_SIZE;
  return new_page;
}

static pte*
__continue_walk_create(pte* root, uintptr_t addr, pte* pte)
{
  uintptr_t new_page = get_new_page();

  unsigned long free_ppn = ppn(new_page);
  *pte = ptd_create(free_ppn);
  return __walk_create(root, addr);
}

static pte*
__walk_internal(pte* root, uintptr_t addr, int create)
{
  pte* t = root;
  int i;
  for (i = 1; i < RISCV_PT_LEVELS; i++)
  {
    size_t idx = RISCV_GET_PT_INDEX(addr, i);

    if (!(t[idx] & PTE_V))
      return create ? __continue_walk_create(root, addr, &t[idx]) : 0;

    t = (pte*) (pte_ppn(t[idx]) << RISCV_PAGE_BITS); 
  }

  return &t[RISCV_GET_PT_INDEX(addr, 3)];
}

/* walk the page table and return PTE
 * return 0 if no mapping exists */
static pte*
__walk(pte* root, uintptr_t addr)
{
  return __walk_internal(root, addr, 0);
}

/* walk the page table and return PTE
 * create the mapping if non exists */
static pte*
__walk_create(pte* root, uintptr_t addr)
{
  return __walk_internal(root, addr, 1);
}


uint32_t 
mapPage(uintptr_t va, uintptr_t pa) {
  pte* pte = __walk_create(root_page_table, va);

  // TODO: what is supposed to happen if page is already allocated?
  if (*pte & PTE_V) {
    return -1;
  }

  uintptr_t phys_page_num = ppn(pa);

  *pte = pte_create(phys_page_num, PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V);
  return 1;
}

uint32_t
allocPage(uintptr_t va, uintptr_t src) {
  uintptr_t page_addr;
  // uintptr_t* pFreeList = (uintptr_t*)freeList; 

  pte* pte = __walk_create(root_page_table, va);

  /* if the page has been already allocated, return the page */
  if (*pte & PTE_V) {
    return PAGE_DOWN(va);
  }

  /* otherwise, allocate one from EPM freeList */
  page_addr = get_new_page();
  uintptr_t phys_page_num = ppn(page_addr);

  *pte = pte_create(phys_page_num, PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V);
  if (src != (uintptr_t) NULL) {
    memcpy((void*) page_addr, (void*) src, RISCV_PAGE_SIZE);
  } else {
    memset((void*) page_addr, 0, RISCV_PAGE_SIZE);
  }
  return page_addr;
}

/* This function pre-allocates the required page tables so that
 * the virtual addresses are linearly mapped to the physical memory */
size_t
epmAllocVspace(uintptr_t addr, size_t num_pages) {
  size_t count;

  for (count = 0; count < num_pages; count++, addr += RISCV_PAGE_SIZE) {
    pte* pte = __walk_create(root_page_table, addr);
    if (!pte) break;
  }

  return count;
}

uintptr_t
epm_va_to_pa(uintptr_t addr) {
  pte* pte = __walk(root_page_table, addr);
  if (pte)
    return pte_ppn(*pte) << RISCV_PAGE_BITS;
  else
    return 0;
}

void
__map_with_reserved_page_table_32(uintptr_t dram_base,
                               uintptr_t dram_size,
                               uintptr_t ptr,
                               pte* l2_pt)
{
  uintptr_t offset = 0;
  uintptr_t leaf_level = 2;
  pte* leaf_pt = l2_pt;
  unsigned long dram_max =  RISCV_GET_LVL_PGSIZE(leaf_level - 1);

  /* use megapage if l2_pt is null */
  if (!l2_pt) {
    leaf_level = 1;
    leaf_pt = root_page_table;
    dram_max = -1UL; 
  }

  assert(dram_size <= dram_max);
  assert(IS_ALIGNED(dram_base, RISCV_GET_LVL_PGSIZE_BITS(leaf_level)));
  assert(IS_ALIGNED(ptr, RISCV_GET_LVL_PGSIZE_BITS(leaf_level - 1)));

  if(l2_pt) {
       /* set root page table entry */
       root_page_table[RISCV_GET_PT_INDEX(ptr, 1)] =
       ptd_create(ppn((uintptr_t) l2_pt));
  }

  for (offset = 0;
       offset < dram_size;
       offset += RISCV_GET_LVL_PGSIZE(leaf_level))
  {
        leaf_pt[RISCV_GET_PT_INDEX(ptr + offset, leaf_level)] =
        pte_create(ppn(dram_base + offset),
                 PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
  }

}

void
__map_with_reserved_page_table_64(uintptr_t dram_base,
                               uintptr_t dram_size,
                               uintptr_t ptr,
                               pte* l2_pt,
                               pte* l3_pt)
{
  uintptr_t offset = 0;
  uintptr_t leaf_level = 3;
  pte* leaf_pt = l3_pt;
  /* use megapage if l3_pt is null */
  if (!l3_pt) {
    leaf_level = 2;
    leaf_pt = l2_pt;
  }
  assert(dram_size <= RISCV_GET_LVL_PGSIZE(leaf_level - 1));
  assert(IS_ALIGNED(dram_base, RISCV_GET_LVL_PGSIZE_BITS(leaf_level)));
  assert(IS_ALIGNED(ptr, RISCV_GET_LVL_PGSIZE_BITS(leaf_level - 1)));

  /* set root page table entry */
  root_page_table[RISCV_GET_PT_INDEX(ptr, 1)] =
    ptd_create(ppn((uintptr_t) l2_pt));

  /* set L2 if it's not leaf */
  if (leaf_pt != l2_pt) {
    l2_pt[RISCV_GET_PT_INDEX(ptr, 2)] =
      ptd_create(ppn((uintptr_t) l3_pt));
  }

  /* set leaf level */
  for (offset = 0;
       offset < dram_size;
       offset += RISCV_GET_LVL_PGSIZE(leaf_level))
  {
    leaf_pt[RISCV_GET_PT_INDEX(ptr + offset, leaf_level)] =
      pte_create(ppn(dram_base + offset),
          PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
  }

}

void
map_with_reserved_page_table(uintptr_t dram_base,
                             uintptr_t dram_size,
                             uintptr_t ptr,
                             pte* l2_pt,
                             pte* l3_pt)
{
  #if __riscv_xlen == 64
  if (dram_size > RISCV_GET_LVL_PGSIZE(2))
    __map_with_reserved_page_table_64(dram_base, dram_size, ptr, l2_pt, 0);
  else
    __map_with_reserved_page_table_64(dram_base, dram_size, ptr, l2_pt, l3_pt);
  #elif __riscv_xlen == 32
  if (dram_size > RISCV_GET_LVL_PGSIZE(1))
    __map_with_reserved_page_table_32(dram_base, dram_size, ptr, 0);
  else
    __map_with_reserved_page_table_32(dram_base, dram_size, ptr, l2_pt);
  #endif
}
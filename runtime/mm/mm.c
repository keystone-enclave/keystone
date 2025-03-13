#include "mm/common.h"
#include "mm/mm.h"
#include "mm/vm.h"
#include "mm/freemem.h"
#include "mm/paging.h"
#include "util/rt_util.h"

/* Page table utilities */
static pte*
__walk_create(pte* root, uintptr_t addr);

/* Hacky storage of current u-mode break */
static uintptr_t current_program_break;

uintptr_t get_program_break()
{
  return current_program_break;
}

void set_program_break(uintptr_t new_break)
{
  current_program_break = new_break;
}

static pte*
__continue_walk_create(pte* root, uintptr_t addr, pte* pte)
{
  uintptr_t new_page = spa_get_zero();
  assert(new_page);

  unsigned long free_ppn = ppn(__pa(new_page));
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

    t = (pte*) __va(pte_ppn(t[idx]) << RISCV_PAGE_BITS);
  }

  return &t[RISCV_GET_PT_INDEX(addr, RISCV_PT_LEVELS)];
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

/* Create a virtual memory mapping between a physical and virtual page */
uintptr_t 
map_page(uintptr_t vpn, uintptr_t ppn, int flags)
{
  pte* pte = __walk_create(root_page_table, vpn << RISCV_PAGE_BITS);

  // TODO: what is supposed to happen if page is already allocated?
  if (*pte & PTE_V) {
    return -1;
  }

  *pte = pte_create(ppn, PTE_D | PTE_A | PTE_V | flags);
  return 1;
}

/* allocate a new page to a given vpn
 * returns VA of the page, (returns 0 if fails) */
uintptr_t
alloc_page(uintptr_t vpn, int flags)
{
  uintptr_t page;
  pte* pte = __walk_create(root_page_table, vpn << RISCV_PAGE_BITS);

  if (!pte)
    return 0;

	/* if the page has been already allocated, return the page */
  if(*pte & PTE_V) {
    return __va(*pte << RISCV_PAGE_BITS);
  }

	/* otherwise, allocate one from the freemem */
  page = spa_get_zero();
  assert(page);

  *pte = pte_create(ppn(__pa(page)), PTE_D | PTE_A | PTE_V | flags);
#ifdef USE_PAGING
  paging_inc_user_page();
#endif

  return page;
}

uintptr_t
realloc_page(uintptr_t vpn, int flags)
{
  assert(flags & PTE_U);

  pte *pte = __walk(root_page_table, vpn << RISCV_PAGE_BITS);
  if(!pte)
    return 0;

  if(*pte & PTE_V) {
    *pte = pte_create(pte_ppn(*pte), flags);
    return __va(*pte << RISCV_PAGE_BITS);
  }

  return 0;
}

uintptr_t
unmap_page(uintptr_t vpn)
{
  pte* pte = __walk(root_page_table, vpn << RISCV_PAGE_BITS);

  // No such PTE, or invalid
  if(!pte || !(*pte & PTE_V))
    return 0;

  assert(*pte & PTE_U);

  uintptr_t ppn = pte_ppn(*pte);
  // Mark invalid
  // TODO maybe do more here
  *pte = 0;
  return ppn;
}

void
free_page(uintptr_t vpn){
  uintptr_t ppn = unmap_page(vpn);

  if(ppn) {
#ifdef USE_PAGING
    paging_dec_user_page();
#endif
    // Return phys page
    spa_put(__va(ppn << RISCV_PAGE_BITS));
  }
}

/* allocate n new pages from a given vpn
 * returns the number of pages allocated */
size_t
alloc_pages(uintptr_t vpn, size_t count, int flags)
{
  unsigned int i;
  for (i = 0; i < count; i++) {
    if(!alloc_page(vpn + i, flags))
      break;
  }

  return i;
}

void
free_pages(uintptr_t vpn, size_t count){
  unsigned int i;
  for (i = 0; i < count; i++) {
    free_page(vpn + i);
  }

}

/*
 * Check if a range of VAs contains any allocated pages, starting with
 * the given VA. Returns the number of sequential pages that meet the
 * conditions.
 */
size_t
test_va_range(uintptr_t vpn, size_t count){

  unsigned int i;
  /* Validate the region */
  for (i = 0; i < count; i++) {
    pte* pte = __walk_internal(root_page_table, (vpn+i) << RISCV_PAGE_BITS, 0);
    // If the page exists and is valid then we cannot use it
    if(pte && *pte){
      break;
    }
  }
  return i;
}

static uintptr_t curr_avail_vpn = 0;

uintptr_t find_va_range(size_t count) {
  if(!curr_avail_vpn)
    curr_avail_vpn = vpn(EYRIE_ANON_REGION_START);

  uintptr_t vpn = curr_avail_vpn;
  uintptr_t valid_pages;

  while((vpn + count) <= EYRIE_ANON_REGION_END){
    valid_pages = test_va_range(vpn, count);

    if(count == valid_pages){
      // Set a successful value if we allocate
      curr_avail_vpn = vpn + count;
      return vpn;
    }
    else
      vpn += valid_pages + 1;
  }

  return 0;
}

/* get a mapped physical address for a VA */
uintptr_t
translate(uintptr_t va)
{
  pte* pte = __walk(root_page_table, va);

  if(pte && (*pte & PTE_V))
    return (pte_ppn(*pte) << RISCV_PAGE_BITS) | (RISCV_PAGE_OFFSET(va));
  else
    return 0;
}

/* try to retrieve PTE for a VA, return 0 if fail */
pte*
pte_of_va(uintptr_t va)
{
  pte* pte = __walk(root_page_table, va);
  return pte;
}

uintptr_t map_with_dynamic_page_table(uintptr_t base, uintptr_t size, uintptr_t va, bool user) {
  // Base is PA, ptr is VA
  unsigned int i;
  pte *pte;

  for(i = 0; i < size; i += RISCV_PAGE_SIZE) {
    // Below function will always return a pte, but it may be a preexisting one
    // which would indicate that this virtual address range is already mapped.
    // This is a situation we would not normally expect to see, since find_va_range
    // above is supposed to return unallocated virtual address space. Dealing
    // with this is complicated, so for now we just assert on this not being the
    // case. However, correctly handling this is definitely a TODO

    pte = __walk_create(root_page_table, va + i);
    assert(!(*pte & PTE_V));

    *pte = pte_create(ppn(base + i), PTE_W | PTE_R | PTE_X | PTE_D | PTE_A |
                                         (user ? PTE_U : 0));
  }

  tlb_flush();
  return va;
}

uintptr_t map_anywhere_with_dynamic_page_table(uintptr_t base, uintptr_t size)
{
  uintptr_t virt_pagenum = find_va_range(vpn(PAGE_UP(size)));
  if(virt_pagenum) {
    return map_with_dynamic_page_table(base, size,
                                       (virt_pagenum << RISCV_PAGE_BITS), false);
  } else {
    return 0;
  }
}

void unmap_with_any_page_table(uintptr_t base, uintptr_t size) {
  int i;
  uintptr_t ppn;

  for(i = 0; i < size; i += RISCV_PAGE_SIZE) {
    ppn = unmap_page(vpn(base + i));

    // If no ppn was returned, this was not a valid page mapping
    assert(ppn);
  }

  tlb_flush();
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
       ptd_create(ppn(kernel_va_to_pa(l2_pt)));
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
    ptd_create(ppn(kernel_va_to_pa(l2_pt)));

  /* set L2 if it's not leaf */
  if (leaf_pt != l2_pt) {
    l2_pt[RISCV_GET_PT_INDEX(ptr, 2)] =
      ptd_create(ppn(kernel_va_to_pa(l3_pt)));
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

  tlb_flush();
}


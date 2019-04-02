#include "common.h"
#include "syscall.h"
#include "mm.h"
#include "freemem.h"
#include "vm.h"

/* Page table utilities */
static pte_t*
__walk_create(pte_t* root, uintptr_t addr);

static pte_t*
__continue_walk_create(pte_t* root, uintptr_t addr, pte_t* pte)
{
  uintptr_t new_page = spa_get();
  unsigned long free_ppn = ppn(__pa(new_page));
  *pte = ptd_create(free_ppn);
  return __walk_create(root, addr);
}

static pte_t*
__walk_internal(pte_t* root, uintptr_t addr, int create)
{
  pte_t* t = root;
  int i;
  for (i = 1; i < RISCV_PT_LEVELS; i++)
  {
    size_t idx = RISCV_GET_PT_INDEX(addr, i);

    if (!(t[idx] & PTE_V))
      return create ? __continue_walk_create(root, addr, &t[idx]) : 0;

    t = (pte_t*) __va(pte_ppn(t[idx]) << RISCV_PAGE_BITS);
  }

  return &t[RISCV_GET_PT_INDEX(addr, 3)];
}

/* walk the page table and return PTE
 * return 0 if no mapping exists */
static pte_t*
__walk(pte_t* root, uintptr_t addr)
{
  return __walk_internal(root, addr, 0);
}

/* walk the page table and return PTE
 * create the mapping if non exists */
static pte_t*
__walk_create(pte_t* root, uintptr_t addr)
{
  return __walk_internal(root, addr, 1);
}

/* maps a physical address to the virtual address
 * and returns VA (returns 0 on error)
 * WARNING: the physical page must be accessible and usable */
uintptr_t
remap_physical_page(uintptr_t vpn, uintptr_t ppn, int flags)
{
  pte_t* pte =  __walk_create(root_page_table, vpn << RISCV_PAGE_BITS);

  if (!pte)
    return 0;

  *pte = pte_create(ppn, flags);

  return (vpn << RISCV_PAGE_BITS);
}

/* maps a physical address range to a virtual address range
 * and returns the number of pages mapped
 * WARNING: the physical page must be accessible and usable */
size_t
remap_physical_pages(uintptr_t vpn, uintptr_t ppn, size_t count, int flags)
{
  unsigned int i;

  for (i = 0; i < count; i++) {
    if(!remap_physical_page(vpn + i, ppn + i, flags))
      break;
  }

  return i;
}

/* get a mapped physical address for a VA */
uintptr_t
translate(uintptr_t va)
{
  pte_t* pte = __walk(root_page_table, va);

  if(*pte & PTE_V)
    return pte_ppn(*pte) << RISCV_PAGE_BITS;
  else
    return 0;
}

/* allocate a new page to a given vpn
 * returns VA of the page, (returns 0 if fails) */
uintptr_t
alloc_page(uintptr_t vpn, int flags)
{
  uintptr_t page;
  pte_t* pte = __walk_create(root_page_table, vpn << RISCV_PAGE_BITS);

  if (!pte)
    return 0;

	/* if the page has been already allocated, return the page */
  if(*pte & PTE_V) {
    return __va(*pte << RISCV_PAGE_BITS);
  }

	/* otherwise, allocate one from the freemem */
  page = spa_get();
  *pte = pte_create(ppn(__pa(page)), flags | PTE_V);

  return page;
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

uintptr_t syscall_mmap(void *addr, size_t length, int prot, int flags,
                 int fd, __off_t offset){

  print_strace("[runtime] [mmap]: addr: %p, length %lu, prot 0x%x, flags 0x%x, fd %i, offset %lu\r\n", addr, length, prot, flags, fd, offset);
  return (uintptr_t)((void*)-1);
}

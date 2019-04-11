#ifdef USE_FREEMEM
#include "common.h"
#include "vm.h"
#include "freemem.h"

/* This file implements a simple page allocator (SPA)
 * which stores the pages based on a linked list.
 * Each of the element of linked list is stored in the header of each free page.

 * SPA does not use any additional data structure (e.g., linked list).
 * Instead, it uses the pages themselves to store the linked list pointer.
 * Thus, each of the free pages contains the pointer to the next free page
 * which can be dereferenced by NEXT_PAGE() macro.
 * spa_free_pages will only hold the head and the tail pages so that
 * SPA can allocate/free a page in constant time. */

static pg_list_t spa_free_pages;

/* get a free page from the simple page allocator */
uintptr_t
spa_get(void)
{
  uintptr_t free_page;

  if (LIST_EMPTY(spa_free_pages)) {
    printf("eyrie simple page allocator runs out of free pages %s","\n");
    return 0;
  }

  free_page = spa_free_pages.head;
  assert(free_page);

  /* update list head */
  uintptr_t next = NEXT_PAGE(spa_free_pages.head);
  spa_free_pages.head = next;
  spa_free_pages.count--;

  return free_page;
}

/* put a page to the simple page allocator */
void
spa_put(uintptr_t page_addr)
{
  uintptr_t prev;

  assert(IS_ALIGNED(page_addr, RISCV_PAGE_BITS));
  assert(page_addr >= freemem_va_start && page_addr < (freemem_va_start  + freemem_size));

  if (!LIST_EMPTY(spa_free_pages)) {
    prev = spa_free_pages.tail;
    assert(prev);
    NEXT_PAGE(prev) = page_addr;
  } else {
    spa_free_pages.head = page_addr;
  }

  NEXT_PAGE(page_addr) = 0;
  spa_free_pages.tail = page_addr;

  spa_free_pages.count++;
  return;
}

unsigned int
spa_available(){
  return spa_free_pages.count;
}

void
spa_init(uintptr_t base, size_t size)
{
  uintptr_t cur;

  LIST_INIT(spa_free_pages);

  // both base and size must be page-aligned
  assert(IS_ALIGNED(base, RISCV_PAGE_BITS));
  assert(IS_ALIGNED(size, RISCV_PAGE_BITS));

  /* put all free pages in freemem (base) into spa_free_pages */
  for(cur = base;
      cur < base + size;
      cur += RISCV_PAGE_SIZE) {
    spa_put(cur);
  }
}
#endif // USE_FREEMEM

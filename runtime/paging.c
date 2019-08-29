#if defined(USE_PAGING) && !defined(USE_FREEMEM)
#error "paging requires freemem"
#endif

#if defined(USE_FREEMEM) && defined(USE_PAGING)

#include "paging.h"

uintptr_t paging_backing_storage_addr;
uintptr_t paging_backing_storage_size;
uintptr_t paging_next_backing_page_offset;

static uintptr_t paging_user_page_count = 0;

extern uintptr_t rt_trap_table;

void paging_inc_user_page(void)
{
  paging_user_page_count++;
}

void paging_dec_user_page(void)
{
  paging_user_page_count--;
  assert(paging_user_page_count >= 0);
}

uintptr_t __alloc_backing_page()
{
  uintptr_t next_page;

  /* no backing page available */
  if (paging_next_backing_page_offset >= paging_backing_storage_size) {
    warn("no backing page avaiable");
    return 0;
  }

  next_page = paging_backing_storage_addr + paging_next_backing_page_offset;
  assert(IS_ALIGNED(next_page, RISCV_PAGE_BITS));

  paging_next_backing_page_offset += RISCV_PAGE_SIZE;

  return next_page;
}

unsigned int
paging_remaining_pages()
{
  return (paging_backing_storage_size - paging_next_backing_page_offset)/RISCV_PAGE_SIZE;
}

void init_paging(uintptr_t user_pa_start, uintptr_t user_pa_end)
{
  uintptr_t addr;
  uintptr_t size;
  uintptr_t* trap_table = &rt_trap_table;

  /* query if there is backing store */
  size = MEGAPAGE_DOWN(sbi_query_multimem());

  if (!size) {
		warn("no backing store found\n");
    return;
  }

  /* query the backing store PA */
  addr = MEGAPAGE_UP(sbi_query_multimem_addr());

  if (!addr) {
		warn("address is zero\n");
    return;
  }

  paging_pa_start = addr;
  paging_backing_storage_size = size;
  paging_backing_storage_addr = __paging_va(addr);
  paging_next_backing_page_offset = 0;

  debug("BACK: 0x%lx-0x%lx (%u KB), va 0x%lx", addr, addr + size, size/1024, paging_backing_storage_addr);

  /* create VA mapping, we don't give execution perm */
  map_with_reserved_page_table(addr, size, EYRIE_PAGING_START, paging_l2_page_table, paging_l3_page_table);
  /*
  remap_physical_pages(vpn(EYRIE_PAGING_START),
                       ppn(addr), size >> RISCV_PAGE_BITS,
                       PTE_R | PTE_W | PTE_D | PTE_A);
  */
  /* register page fault handler */
  trap_table[RISCV_EXCP_INST_PAGE_FAULT] = (uintptr_t) paging_handle_page_fault;
  trap_table[RISCV_EXCP_LOAD_PAGE_FAULT] = (uintptr_t) paging_handle_page_fault;
  trap_table[RISCV_EXCP_STORE_PAGE_FAULT] = (uintptr_t) paging_handle_page_fault;

  paging_user_page_count = (user_pa_end - user_pa_start) >> RISCV_PAGE_BITS;

  return;
}

uintptr_t
__traverse_page_table_and_pick_internal(
    int level,
    pte* tb,
    uintptr_t vaddr,
    uintptr_t *count)
{
  pte* walk;
  int i=0;

  assert(count);

  for (walk=tb, i=0;
       walk < tb + (RISCV_PAGE_SIZE/sizeof(pte));
       walk += 1, i++)
  {
    if(*walk == 0)
      continue;

    pte entry = *walk;
    uintptr_t phys_addr = pte_ppn(entry) << RISCV_PAGE_BITS;
    uintptr_t virt_addr = ((vaddr << RISCV_PT_INDEX_BITS) | (i&0x1ff))<<RISCV_PAGE_BITS;

    /* if this is a leaf */
    if(level == 1 ||
        (entry & PTE_R) || (entry & PTE_W) || (entry & PTE_X))
    {
      if ((entry & PTE_U) && (entry & PTE_V))
      {
        *count = *count - 1;
      }

      if (*count == 0)
        return virt_addr;
    }
    else
    {
      uintptr_t ret = 0;

      /* extending MSB */
      if(level == 3 && (i&0x100))
        vaddr = 0xffffffffffffffffUL;

      ret = __traverse_page_table_and_pick_internal(
          level - 1,
          (pte*) __va(phys_addr),
          vpn(virt_addr),
          count);
      if (ret)
        return ret;
    }
  }

  return 0;
}

uintptr_t
__traverse_page_table_and_pick(uintptr_t count)
{
  uintptr_t next_count = count;
  uintptr_t ret = 0;
  int try = 0;
  int max_retry = 3;

  while (next_count > 0 && try < max_retry)
  {
    ret = __traverse_page_table_and_pick_internal(
      RISCV_PT_LEVELS, root_page_table, 0, &next_count);
    try++;
  }

  return ret;
}

/* pick a virtual page to evict
 * at this moment, we randomly choose a user page
 * return: va of a page mapped to user
 *         0 if failed */
uintptr_t __pick_page()
{
  uintptr_t target = 0;

  uintptr_t rnd;
  uintptr_t count;

  assert(paging_user_page_count > 0);
  rnd = sbi_random();
  count = (rnd % paging_user_page_count) + 1;
  target = __traverse_page_table_and_pick(count);

  return target;
}

/* evict a page from EPM and store it to the backing storage
 * back_page (PA1) <-- epm_page (PA2) <-- swap_page (PA1)
 * if swap_page is 0, no need to write epm_page
 */
void __swap_epm_page(uintptr_t back_page, uintptr_t epm_page, uintptr_t swap_page, bool encrypt)
{
  assert(epm_page >= EYRIE_LOAD_START);
  assert(epm_page < freemem_va_start + freemem_size);
  assert(back_page >= paging_backing_storage_addr);
  assert(back_page < paging_backing_storage_addr + paging_backing_storage_size);

  /* not implemented */
  assert(!encrypt);

  char buffer[RISCV_PAGE_SIZE] = {0,};
  if (swap_page) {
    assert(swap_page == back_page);
    memcpy(buffer, (void*)swap_page, RISCV_PAGE_SIZE);
  }

  memcpy((void*)back_page, (void*)epm_page, RISCV_PAGE_SIZE);

  if (swap_page) {
    memcpy((void*)epm_page, buffer, RISCV_PAGE_SIZE);
  }

  return;
}

/* pick a user page, evict, and put it to the freemem
 * input: backing store addr (va)
 *        0 if new
 * return: loaded frame address (pa)
 *        0 if failed */
uintptr_t paging_evict_and_free_one(uintptr_t swap_va)
{
  /* pick a valid page */
  uintptr_t target_va, dest_va, src_pa;
  pte* target_pte;

  target_va = __pick_page();

  if(!target_va) {
    warn("**** failed to pick frame to evict");
    return 0;
  }

  /* find the destination to swap out */
  if(swap_va)
    dest_va = swap_va;
  else
    dest_va = __alloc_backing_page();

  assert(dest_va >= paging_backing_storage_addr);
  assert(dest_va < paging_backing_storage_addr +
                   paging_backing_storage_size);

  /* evict & load */
  target_pte = pte_of_va(target_va);
  assert(target_pte && (*target_pte & PTE_U));

  src_pa = pte_ppn(*target_pte) << RISCV_PAGE_BITS;
  __swap_epm_page(dest_va, __va(src_pa), swap_va, false);

  /* invalidate target PTE */
  *target_pte = pte_create_invalid(ppn(__paging_pa(dest_va)),
      *target_pte & PTE_FLAG_MASK);
  paging_dec_user_page();

  tlb_flush();

  return src_pa;
}

void paging_handle_page_fault(struct encl_ctx* ctx)
{
  uintptr_t addr;
  uintptr_t back_ptr;
  uintptr_t frame;
  pte* entry;

  addr = ctx->sbadaddr;

  /* VA legitimacy check */
  if (addr >= EYRIE_LOAD_START)
    goto exit;

  entry = pte_of_va(addr);

  /* VA is never mapped, exit */
  if (!entry)
    goto exit;

  /* if PTE is already valid, it means something went wrong */
  if (*entry & PTE_V)
    goto exit;

  /* where is the page? */
  back_ptr = __paging_va(pte_ppn(*entry) << RISCV_PAGE_BITS);
  if (!back_ptr)
    goto exit;

  assert(back_ptr >= paging_backing_storage_addr);
  assert(back_ptr < paging_backing_storage_addr + paging_backing_storage_size);

  /* evict & swap */
  frame = paging_evict_and_free_one(back_ptr);
  if (!frame)
    goto exit;

  assert(*entry & PTE_U);
  /* validate the entry */
  *entry = pte_create(ppn(frame), *entry & PTE_FLAG_MASK);
  paging_inc_user_page();

  return;
exit:
  warn("fatal paging failure");
  rt_page_fault(ctx);
}

#endif //USE_FREEMEM

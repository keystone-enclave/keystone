#if defined(USE_FREEMEM) && defined(USE_PAGING)

#ifndef __PAGING_H__
#define __PAGING_H__

#include <stdint.h>

#include "common.h"
#include "freemem.h"
#include "mm.h"
#include "printf.h"
#include "regs.h"
#include "rt_util.h"
#include "string.h"
#include "vm_defs.h"

unsigned int paging_remaining_pages(void);
void init_paging(uintptr_t user_pa_start, uintptr_t user_pa_end);
void paging_handle_page_fault(struct encl_ctx* ctx);
uintptr_t paging_evict_and_free_one(uintptr_t swap_va);

extern uintptr_t paging_pa_start;
extern pte paging_l2_page_table[BIT(RISCV_PT_INDEX_BITS)]
    __attribute__((aligned(RISCV_PAGE_SIZE)));
extern pte paging_l3_page_table[BIT(RISCV_PT_INDEX_BITS)]
    __attribute__((aligned(RISCV_PAGE_SIZE)));

void paging_inc_user_page(void);
void paging_dec_user_page(void);
/* page tables for loading physical memory */
static inline uintptr_t __paging_pa(uintptr_t va)
{
  return (va - EYRIE_PAGING_START) + paging_pa_start;
}

static inline uintptr_t __paging_va(uintptr_t pa)
{
  return (pa - paging_pa_start) + EYRIE_PAGING_START;
}

uintptr_t
paging_alloc_backing_page(void);

uintptr_t
paging_backing_region(void);
uintptr_t
paging_backing_region_size(void);
bool
paging_epm_inbounds(uintptr_t addr);
bool
paging_backpage_inbounds(uintptr_t addr);

#endif
#endif

#ifndef __VM_H__
#define __VM_H__

#include <asm/csr.h>

#include "common.h"
#include "printf.h"
#include "vm_defs.h"

extern void* rt_base;

uintptr_t runtime_va_start;
/* Eyrie is for Sv39 */
static inline uintptr_t satp_new(uintptr_t pa)
{
  return (SATP_MODE | (pa >> RISCV_PAGE_BITS));
}

uintptr_t kernel_offset;
static inline uintptr_t kernel_va_to_pa(void* ptr)
{
  return (uintptr_t) ptr - kernel_offset;
}

uintptr_t load_pa_start;
static inline uintptr_t __va(uintptr_t pa)
{
  return (pa - load_pa_start) + EYRIE_LOAD_START;
}

static inline uintptr_t __pa(uintptr_t va)
{
  return (va - EYRIE_LOAD_START) + load_pa_start;
}

static inline pte pte_create(uintptr_t ppn, int type)
{
  return (pte)((ppn << PTE_PPN_SHIFT) | PTE_V | (type & PTE_FLAG_MASK) );
}

static inline pte pte_create_invalid(uintptr_t ppn, int type)
{
  return (pte)((ppn << PTE_PPN_SHIFT) | (type & PTE_FLAG_MASK & ~PTE_V));
}

static inline pte ptd_create(uintptr_t ppn)
{
  return pte_create(ppn, PTE_V);
}

static inline uintptr_t ppn(uintptr_t pa)
{
  return pa >> RISCV_PAGE_BITS;
}

// this is identical to ppn, but separate it to avoid confusion between va/pa
static inline uintptr_t vpn(uintptr_t va)
{
  return va >> RISCV_PAGE_BITS;
}

static inline uintptr_t pte_ppn(pte pte)
{
  return pte >> PTE_PPN_SHIFT;
}



#ifdef USE_FREEMEM
/* root page table */
pte root_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
/* page tables for kernel remap */
pte kernel_l2_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
pte kernel_l3_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
/* page tables for loading physical memory */
pte load_l2_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
pte load_l3_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));

/* Program break */
uintptr_t program_break;

/* freemem */
uintptr_t freemem_va_start;
size_t freemem_size;
#endif // USE_FREEMEM

/* shared buffer */
uintptr_t shared_buffer;
uintptr_t shared_buffer_size;


#endif

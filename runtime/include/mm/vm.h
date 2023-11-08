#ifndef __VM_H__
#define __VM_H__

#include "mm/common.h"
#include "util/printf.h"
#include "mm/vm_defs.h"

extern uintptr_t runtime_va_start;

#ifdef LOADER_BIN

#include "csr.h"

/* root page table */
extern pte root_page_table[];
/* page tables for loading physical memory */
extern pte load_l2_page_table[];
extern pte load_l3_page_table[];

/* no-ops */

static inline uintptr_t kernel_va_to_pa(void* ptr)
{
  return (uintptr_t) ptr;
}

static inline uintptr_t __va(uintptr_t pa)
{
  return pa;
}

static inline uintptr_t __pa(uintptr_t va)
{
  return va;
}

#else

/* root page table */
extern pte* root_page_table;

extern void* rt_base;
extern uintptr_t kernel_offset; // TODO: is this needed?
extern uintptr_t load_pa_start;

static inline uintptr_t kernel_va_to_pa(void* ptr)
{
  return (uintptr_t) ptr - kernel_offset;
}

static inline uintptr_t __va(uintptr_t pa)
{
  return (pa - load_pa_start) + EYRIE_LOAD_START;
}

static inline uintptr_t __pa(uintptr_t va)
{
  return (va - EYRIE_LOAD_START) + load_pa_start;
}

/* Program break */
extern uintptr_t program_break;

/* freemem */
extern uintptr_t freemem_va_start;
extern size_t freemem_size;

/* shared buffer */
extern uintptr_t shared_buffer;
extern uintptr_t shared_buffer_size;

#endif

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

#endif

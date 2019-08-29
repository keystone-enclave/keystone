#ifndef __VM_H__
#define __VM_H__

#include <asm/csr.h>
#include "printf.h"
#include "common.h"

#define BIT(n) (1ul << (n))
#define MASK(n) (BIT(n)-1ul)
#define IS_ALIGNED(n, b) (!((n) & MASK(b)))

#define RISCV_PT_INDEX_BITS 9
#define RISCV_PT_LEVELS 3
#define RISCV_PAGE_BITS 12
#define RISCV_PAGE_SIZE (1<<RISCV_PAGE_BITS)
#define RISCV_PAGE_OFFSET(addr) (addr % RISCV_PAGE_SIZE)
#define RISCV_GET_PT_INDEX(addr, n)                                     \
  (((addr) >> (((RISCV_PT_INDEX_BITS) * ((RISCV_PT_LEVELS) - (n))) + RISCV_PAGE_BITS)) \
   & MASK(RISCV_PT_INDEX_BITS))
#define RISCV_GET_LVL_PGSIZE_BITS(n) (((RISCV_PT_INDEX_BITS) * (RISCV_PT_LEVELS - (n))) + RISCV_PAGE_BITS)
#define RISCV_GET_LVL_PGSIZE(n)      BIT(RISCV_GET_LVL_PGSIZE_BITS((n)))

#define ROUND_UP(n, b) (((((n) - 1ul) >> (b)) + 1ul) << (b))
#define ROUND_DOWN(n, b) (n & ~((2 << (b-1)) - 1))
#define PAGE_DOWN(n) ROUND_DOWN(n, RISCV_PAGE_BITS)
#define PAGE_UP(n) ROUND_UP(n, RISCV_PAGE_BITS)
#define MEGAPAGE_DOWN(n) ROUND_DOWN(n, RISCV_GET_LVL_PGSIZE_BITS(2))
#define MEGAPAGE_UP(n) ROUND_UP(n, RISCV_GET_LVL_PGSIZE_BITS(2))

/* Starting address of the enclave memory */
#define EYRIE_LOAD_START        0xffffffff00000000
#define EYRIE_PAGING_START      0xffffffff40000000
#define EYRIE_UNTRUSTED_START   0xffffffff80000000
#define EYRIE_USER_STACK_START  0x0000000040000000
#define EYRIE_ANON_REGION_START 0x0000002000000000 // Arbitrary VA to start looking for large mappings
#define EYRIE_ANON_REGION_END   EYRIE_LOAD_START
#define EYRIE_USER_STACK_SIZE   0x20000
#define EYRIE_USER_STACK_END    (EYRIE_USER_STACK_START - EYRIE_USER_STACK_SIZE)

#define PTE_V     0x001 // Valid
#define PTE_R     0x002 // Read
#define PTE_W     0x004 // Write
#define PTE_X     0x008 // Execute
#define PTE_U     0x010 // User
#define PTE_G     0x020 // Global
#define PTE_A     0x040 // Accessed
#define PTE_D     0x080 // Dirty
#define PTE_FLAG_MASK 0x3ff
#define PTE_PPN_SHIFT 10

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

typedef uintptr_t pte;
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

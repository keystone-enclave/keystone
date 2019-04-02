#ifndef __VM_H__
#define __VM_H__


#include <asm/csr.h>
#include "printf.h"
#include "common.h"

#ifndef WITHOUT_FREEMEM

#define BIT(n) (1ul << (n))
#define MASK(n) (BIT(n)-1ul)
#define IS_ALIGNED(n, b) (!((n) & MASK(b)))

#define RISCV_PT_INDEX_BITS 9
#define RISCV_PT_LEVELS 3
#define RISCV_PAGE_BITS 12
#define RISCV_PAGE_SIZE (1<<RISCV_PAGE_BITS)
#define RISCV_GET_PT_INDEX(addr, n) \
  (((addr) >> (((RISCV_PT_INDEX_BITS) * ((RISCV_PT_LEVELS) - (n))) + RISCV_PAGE_BITS)) \
   & MASK(RISCV_PT_INDEX_BITS))
#define RISCV_GET_LVL_PGSIZE_BITS(n) (((RISCV_PT_INDEX_BITS) * (RISCV_PT_LEVELS - (n))) + RISCV_PAGE_BITS)
#define RISCV_GET_LVL_PGSIZE(n)      BIT(RISCV_GET_LVL_PGSIZE_BITS((n)))

/* Starting address of the enclave memory */
#define EYRIE_LOAD_START 0xffffffff00000000

#define PTE_V     0x001 // Valid
#define PTE_R     0x002 // Read
#define PTE_W     0x004 // Write
#define PTE_X     0x008 // Execute
#define PTE_U     0x010 // User
#define PTE_G     0x020 // Global
#define PTE_A     0x040 // Accessed
#define PTE_D     0x080 // Dirty
#define PTE_PPN_SHIFT 10

extern void* rt_base;

uintptr_t runtime_va_start;
/* Eyrie is for Sv39 */
static inline uintptr_t satp_new(uintptr_t pa)
{
  return (SATP_MODE | (pa >> RISCV_PAGE_BITS));
}

static uintptr_t kernel_offset;
static inline uintptr_t kernel_va_to_pa(void* ptr)
{
  return (uintptr_t) ptr - kernel_offset;
}

static uintptr_t load_pa_start;
static inline uintptr_t __va(uintptr_t pa)
{
  return (pa - load_pa_start) + EYRIE_LOAD_START;
}

static inline uintptr_t __pa(uintptr_t va)
{
  return (va - EYRIE_LOAD_START) + load_pa_start;
}

typedef uintptr_t pte_t;
static inline pte_t pte_create(uintptr_t ppn, int type)
{
  return (pte_t)((ppn << PTE_PPN_SHIFT) | PTE_V | type );
}

static inline pte_t ptd_create(uintptr_t ppn)
{
  return pte_create(ppn, PTE_V);
}

/* freemem */
uintptr_t freemem_va_start;
size_t freemem_size;

#endif // WITHOUT_FREEMEM

/* shared buffer */
uintptr_t shared_buffer;
uintptr_t shared_buffer_size;


#endif

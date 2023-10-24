#include <stddef.h>
#include <stdint.h>
#include "vm.h"

// TODO: figure out which of these we need

uintptr_t runtime_va_start;
uintptr_t kernel_offset;
uintptr_t load_pa_start;

/* root page table */
pte root_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
/* page tables for loading physical memory */
pte load_l2_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
pte load_l3_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));

/* Program break */
uintptr_t program_break;

/* freemem */
uintptr_t freemem_va_start;
size_t freemem_size;

/* shared buffer */
uintptr_t shared_buffer;
uintptr_t shared_buffer_size;



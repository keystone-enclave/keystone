#include <asm/csr.h>

#include "printf.h"
#include "interrupt.h"
#include "syscall.h"
#include "vm.h"
#include "sbi.h"

/* defined in syscall.c */
extern uintptr_t shared_buffer;
extern uintptr_t shared_buffer_size;

/* initial memory layout */
uintptr_t freemem_base;
size_t freemem_size;
uintptr_t utm_base;
size_t utm_size;

/* defined in entry.S */
extern void* encl_trap_handler;

void
init_freemem(uintptr_t freemem_base,
             size_t freemem_size)
{
  /* initialize free memory with a simple page allocator*/

}

/* page tables for kernel remap */
pte_t root_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
pte_t l2_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
pte_t l3_page_table[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));

/* map the entire physical addresses to the remap range */
void remap_kernel_space(uintptr_t runtime_base,
                        uintptr_t runtime_size)
{
  uintptr_t offset;

  if (runtime_size > RISCV_GET_LVL_PGSIZE(2))
  {
    printf("Eyrie runtime is supposed to be smaller than %lu", RISCV_GET_LVL_PGSIZE(2));
    sbi_exit_enclave(-1);
  }

  /* root page table */
  root_page_table[RISCV_GET_PT_INDEX(runtime_va_start, 1)] = ptd_create(kernel_va_to_pa(l2_page_table) >> RISCV_PAGE_BITS);

  /* L2 page talbe */
  l2_page_table[RISCV_GET_PT_INDEX(runtime_va_start, 2)] = ptd_create(kernel_va_to_pa(l3_page_table) >> RISCV_PAGE_BITS);

  for (offset = 0; offset < runtime_size; offset += RISCV_GET_LVL_PGSIZE(3))
  {
    l3_page_table[RISCV_GET_PT_INDEX(runtime_va_start + offset, 3)] =
      pte_create((runtime_base + offset) >> RISCV_PAGE_BITS, PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
  }

  printf("writing satp... 0x%lx\n", kernel_va_to_pa(root_page_table));
  csr_write(satp, satp_new(kernel_va_to_pa(root_page_table)));
  printf("done! %d\n",1);
  /*
  if (dram_size > RISCV_GET_LVL_PGSIZE(1))
  {
    // We could have used gigapages in this case, but the memory is probably not aligned.
    // So we exit the enclave because we cannot handle that
    printf("Eyrie runtime does not support enclave larger than %d bytes\n", RISCV_GET_LVL_PGSIZE(1));
    sbi_exit_enclave(-1);
  }
  else if (dram_size > RISCV_GET_LVL_PGSIZE(2))
  {
    uintptr_t offset;
    // the enclave is supposed to be aligned to the 2nd level
    // page table granularity (2 MB) so that we can use megapages
    if (!IS_ALIGNED(dram_base, RISCV_GET_LVL_PGSIZE_BITS(2))) {
      printf("[eyrie] memory address not properly algiend");
      sbi_exit_enclave(-1);
    }

    // root page table
    root_page_table[RISCV_GET_PT_INDEX(va, 1)] = ptd_create(kernel_va_to_pa(l2_page_table));

    // L2 Megapages
    for (offset = 0; offset < dram_size; offset += RISCV_GET_LVL_PGSIZE(2))
    {
      l2_page_table[RISCV_GET_PT_INDEX(va + offset, 2)] = pte_create(dram_base + offset, PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
    }
  }
  // Enclaves smaller than 2MB
  else {

  }
  for (va = EYRIE_REMAP_START, i = RISCV_GET_PT_INDEX(va, 1);
       va < EYRIE_REMAP_END;
       va += RISCV_GET_LVL_PGSIZE(1), i++) {

    // mapping level 1
    root_page_table[BIT(RISCV_PT_INDEX_BITS)] = ptd_create(kernel_va_to_pa(l2_page_table));
  }
  */
}

void
eyrie_boot(uintptr_t dummy, // $a0 contains the return value from the SBI
           uintptr_t dram_base,
           uintptr_t dram_size,
           uintptr_t runtime_paddr,
           uintptr_t user_paddr,
           uintptr_t free_paddr,
           uintptr_t utm_vaddr,
           uintptr_t utm_size)
{
  /* set initial values */
  freemem_base = free_paddr;
  freemem_size = dram_base + dram_size - free_paddr;
  shared_buffer = utm_vaddr;
  shared_buffer_size = utm_size;
  runtime_va_start = (uintptr_t) &rt_base;
  kernel_offset = runtime_va_start - runtime_paddr;

  printf("dram_base: 0x%lx\n", dram_base);
  printf("runtime_paddr: 0x%lx\n", runtime_paddr);
  printf("user_paddr: 0x%lx\n", user_paddr);

  remap_kernel_space(runtime_paddr, user_paddr - runtime_paddr);

  /* set trap vector */
  csr_write(stvec, &encl_trap_handler);

  /* set timer */
  init_timer();

  /* prepare edge & system calls */
  init_edge_internals();
}

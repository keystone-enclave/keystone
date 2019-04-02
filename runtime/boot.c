#include <asm/csr.h>

#include "printf.h"
#include "interrupt.h"
#include "syscall.h"
#include "vm.h"
#include "string.h"
#include "sbi.h"
#include "freemem.h"
#include "mm.h"

/* defined in vm.h */
extern uintptr_t shared_buffer;
extern uintptr_t shared_buffer_size;

/* initial memory layout */
uintptr_t utm_base;
size_t utm_size;

/* defined in entry.S */
extern void* encl_trap_handler;

/* defined in env.c */
extern void* setup_start(void* _sp);

#ifdef USE_FREEMEM

void
map_physical_memory_with_megapages(uintptr_t dram_base,
                                   uintptr_t dram_size,
                                   uintptr_t ptr)
{
  uintptr_t offset = 0;

  /* we're gonna use L2 mega pages, so the memory
   * is supposed to be smaller than a gigapage */
  assert(dram_size <= RISCV_GET_LVL_PGSIZE(1));

  /* if the enclave memory is larger than a megapage,
   * it is supposed to be aligned with a megapage */
  assert(IS_ALIGNED(dram_base, RISCV_GET_LVL_PGSIZE_BITS(2)));

  /* the starting VA must be aligned with a gigapage so that
   * we can use all entries of an L2 page table */
  assert(IS_ALIGNED(ptr, RISCV_GET_LVL_PGSIZE_BITS(1)));

  /* root page table */
  root_page_table[RISCV_GET_PT_INDEX(ptr, 1)] =
    ptd_create(ppn(kernel_va_to_pa(load_l2_page_table)));

  /* map megapages */
  for (offset = 0;
       offset < dram_size;
       offset += RISCV_GET_LVL_PGSIZE(2))
  {
    load_l2_page_table[RISCV_GET_PT_INDEX(ptr + offset, 2)] =
      pte_create(ppn(dram_base + offset),
          PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
  }
}

void
map_physical_memory_with_kilopages(uintptr_t dram_base,
                                   uintptr_t dram_size,
                                   uintptr_t ptr)
{
  uintptr_t offset = 0;

  assert(dram_size <= RISCV_GET_LVL_PGSIZE(2));

  /* the memory is supposed to be aligned with a 4K page */
  assert(IS_ALIGNED(dram_base, RISCV_GET_LVL_PGSIZE_BITS(3)));

  /* the starting VA must be aligned with a megapage so that
   * we can use all entries of a last-level page table */
  assert(IS_ALIGNED(ptr, RISCV_GET_LVL_PGSIZE_BITS(2)));

  /* root page table */
  root_page_table[RISCV_GET_PT_INDEX(ptr, 1)] =
    ptd_create(ppn(kernel_va_to_pa(load_l2_page_table)));

  /* l2 page table */
  load_l2_page_table[RISCV_GET_PT_INDEX(ptr, 2)] =
    ptd_create(ppn(kernel_va_to_pa(load_l3_page_table)));

  /* map pages */
  for (offset = 0;
       offset < dram_size;
       offset += RISCV_GET_LVL_PGSIZE(3))
  {
    load_l3_page_table[RISCV_GET_PT_INDEX(ptr + offset, 3)] =
      pte_create(ppn(dram_base + offset),
          PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
  }
}

/* map entire enclave physical memory so that
 * we can access the old page table and free memory */
/* remap runtime kernel to a new root page table */
void
map_physical_memory(uintptr_t dram_base,
                    uintptr_t dram_size)
{
  uintptr_t ptr = EYRIE_LOAD_START;
  /* load address should not override kernel address */
  assert(RISCV_GET_PT_INDEX(ptr, 1) != RISCV_GET_PT_INDEX(runtime_va_start, 1));

  if (dram_size > RISCV_GET_LVL_PGSIZE(2))
    map_physical_memory_with_megapages(dram_base, dram_size, ptr);
  else
    map_physical_memory_with_kilopages(dram_base, dram_size, ptr);
}

void
remap_kernel_space(uintptr_t runtime_base,
                   uintptr_t runtime_size)
{
  uintptr_t offset;

  /* eyrie runtime is supposed to be smaller than a megapage */
  assert(runtime_size <= RISCV_GET_LVL_PGSIZE(2));

  /* root page table */
  root_page_table[RISCV_GET_PT_INDEX(runtime_va_start, 1)] =
    ptd_create(ppn(kernel_va_to_pa(kernel_l2_page_table)));

  /* L2 page talbe */
  kernel_l2_page_table[RISCV_GET_PT_INDEX(runtime_va_start, 2)] =
    ptd_create(ppn(kernel_va_to_pa(kernel_l3_page_table)));

  for (offset = 0;
       offset < runtime_size;
       offset += RISCV_GET_LVL_PGSIZE(3))
  {
    kernel_l3_page_table[RISCV_GET_PT_INDEX(runtime_va_start + offset, 3)] =
      pte_create(ppn(runtime_base + offset),
          PTE_R | PTE_W | PTE_X | PTE_A | PTE_D);
  }
}

void
copy_root_page_table()
{
  /* the old table lives in the first page */
  pte_t* old_root_page_table = (pte_t*) EYRIE_LOAD_START;
  int i;

  /* copy all valid entries of the old root page table */
  for (i = 0; i < BIT(RISCV_PT_INDEX_BITS); i++) {
    if (old_root_page_table[i] & PTE_V &&
        !(root_page_table[i] & PTE_V)) {
      root_page_table[i] = old_root_page_table[i];
    }
  }
}

/* initialize free memory with a simple page allocator*/
void
init_freemem()
{
  spa_init(freemem_va_start, freemem_size);
}

/* initialize user stack */
void
init_user_stack()
{
  // allocated stack pages right below the runtime
  alloc_pages(vpn(runtime_va_start - EYRIE_USER_STACK_SIZE),
      EYRIE_USER_STACK_SIZE >> RISCV_PAGE_BITS,
      PTE_R | PTE_W | PTE_D | PTE_A | PTE_U);

  // prepare user sp
  csr_write(sscratch, runtime_va_start);
}

#endif // USE_FREEMEM

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
  load_pa_start = dram_base;
  shared_buffer = utm_vaddr;
  shared_buffer_size = utm_size;
  runtime_va_start = (uintptr_t) &rt_base;
  kernel_offset = runtime_va_start - runtime_paddr;
#ifdef USE_FREEMEM
  freemem_va_start = __va(free_paddr);
  freemem_size = dram_base + dram_size - free_paddr;

  /* remap kernel VA */
  remap_kernel_space(runtime_paddr, user_paddr - runtime_paddr);
  map_physical_memory(dram_base, dram_size);

  /* switch to the new page table */
  csr_write(satp, satp_new(kernel_va_to_pa(root_page_table)));

  /* copy valid entries from the old page table */
  copy_root_page_table();

  /* initialize free memory */
  init_freemem();

  /* initialize user stack */
  init_user_stack();

  /* settup user stack env/aux */
  setup_start((void*)runtime_va_start);
#endif // USE_FREEMEM

  /* set trap vector */
  csr_write(stvec, &encl_trap_handler);

  /* prepare edge & system calls */
  init_edge_internals();

  /* set timer */
  init_timer();

  /* booting all finished, droping to the user land */
  return;
}

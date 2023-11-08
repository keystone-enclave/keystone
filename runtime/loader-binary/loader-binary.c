#include "loader/loader.h"
#include "mm/vm.h"
#include "mm/mm.h"
#include "mm/common.h"
#include "mm/freemem.h"
#include "util/printf.h"
#include "../tmplib/asm/csr.h"

/* root page table */
pte root_page_table_storage[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
/* page tables for loading physical memory */
pte load_l2_page_table_storage[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));
pte load_l3_page_table_storage[BIT(RISCV_PT_INDEX_BITS)] __attribute__((aligned(RISCV_PAGE_SIZE)));

uintptr_t free_base_final = 0;

uintptr_t satp_new(uintptr_t pa)
{
  return (SATP_MODE | (pa >> RISCV_PAGE_BITS));
}

void map_physical_memory(uintptr_t dram_base, uintptr_t dram_size) {
  uintptr_t ptr = EYRIE_LOAD_START;
  /* load address should not override kernel address */
  assert(RISCV_GET_PT_INDEX(ptr, 1) != RISCV_GET_PT_INDEX(RUNTIME_VA_START, 1));
  map_with_reserved_page_table(dram_base, dram_size,
      ptr, load_l2_page_table_storage, load_l3_page_table_storage);
}

int load_runtime(uintptr_t dummy,
                uintptr_t dram_base, uintptr_t dram_size, 
                uintptr_t runtime_base, uintptr_t user_base, 
                uintptr_t free_base, uintptr_t untrusted_ptr, 
                uintptr_t untrusted_size) {
  int ret = 0;

  root_page_table = root_page_table_storage;

  // initialize freemem
  spa_init(free_base, dram_base + dram_size - free_base);

  // validate runtime elf 
  size_t runtime_size = user_base - runtime_base;
  if (((void*) runtime_base == NULL) || (runtime_size <= 0)) {
    return -1; 
  }

  // create runtime elf struct
  elf_t runtime_elf;
  ret = elf_newFile((void*) runtime_base, runtime_size, &runtime_elf);
  if (ret < 0) {
    return ret;
  }

  // map runtime memory
  ret = loadElf(&runtime_elf, 0);
  if (ret < 0) {
    return ret;
  }

  // map enclave physical memory, so that runtime will be able to access all memory
  map_physical_memory(dram_base, dram_size);

  // map untrusted memory
  uintptr_t va        = EYRIE_UNTRUSTED_START;
  uintptr_t untr_iter = untrusted_ptr;
  while (va < EYRIE_UNTRUSTED_START + untrusted_size) {
    if (!map_page(vpn(va), ppn(untr_iter), PTE_W | PTE_R)) {
      //return Error::PageAllocationFailure;
      return -1; //TODO: error class later
    }
    va += RISCV_PAGE_SIZE;
    untr_iter += RISCV_PAGE_SIZE;
  }

  free_base_final = dram_base + dram_size - spa_available() * RISCV_PAGE_SIZE;

  return ret;
}

// TODO: exit instead, may need to set up some memory stuff first
void error_and_loop() {
  printf("[loader] ran into error. looping infinitely...\n");
  while(true) {
    printf(".");
  }
}


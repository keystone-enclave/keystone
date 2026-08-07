#include "loader/loader.h"
#include "mm/vm.h"
#include "mm/mm.h"
#include "mm/common.h"
#include "mm/freemem.h"
#include "util/printf.h"
#include <asm/csr.h>
#include "sm_call.h"

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

int map_untrusted_memory(uintptr_t untrusted_ptr, uintptr_t untrusted_size) {
  uintptr_t va        = EYRIE_UNTRUSTED_START;
  while (va < EYRIE_UNTRUSTED_START + untrusted_size) {
    if (!map_page(vpn(va), ppn(untrusted_ptr), PTE_W | PTE_R | PTE_D)) {
      return -1;
    }
    va += RISCV_PAGE_SIZE;
    untrusted_ptr += RISCV_PAGE_SIZE;
  }
  return 0;
}

int load_runtime(uintptr_t dummy,
                uintptr_t dram_base, uintptr_t dram_size,
                uintptr_t free_base, uintptr_t untrusted_base,
                uintptr_t untrusted_size) {
  int ret = 0;

  root_page_table = root_page_table_storage;

  // initialize freemem
  spa_init(free_base, dram_base + dram_size - free_base);

  // find runtime
  resource_ptr_t* runtime_ptr = findIdentityResident(dram_base, MSR_RUNTIME_FILENAME);
  if (!runtime_ptr) {
    return -1;
  }

  // create runtime elf struct
  elf_t runtime_elf;
  ret = elf_newFile((void*) (dram_base + runtime_ptr->offset), runtime_ptr->size, &runtime_elf);
  if (ret != 0) {
    return ret;
  }

  // map runtime memory
  ret = loadElf(&runtime_elf, 0);
  if (ret != 0) {
    return ret;
  }

  // map enclave physical memory, so that runtime will be able to access all memory
  map_physical_memory(dram_base, dram_size);

  // map untrusted memory
  ret = map_untrusted_memory(untrusted_base, untrusted_size);
  if (ret != 0) {
    return ret;
  }

  free_base_final = dram_base + dram_size - spa_available() * RISCV_PAGE_SIZE;

  return ret;
}

void error_and_exit() {
  printf("[loader] FATAL: failed to load.\n");
  sbi_exit_enclave(-1);
}


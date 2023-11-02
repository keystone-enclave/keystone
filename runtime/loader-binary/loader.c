#include "loader.h"
#include "csr.h"
#include "vm.h"
#include "mm.h"
#include "printf.h"
#include "common.h"

void initializeFreeList(uintptr_t freeMemBase, uintptr_t dramBase, size_t dramSize) {
  freeList = freeMemBase;
  epmBase = dramBase; 
  epmSize = dramSize;
}

int pt_mode_from_elf(int elf_pt_mode) {
  return 
    (((elf_pt_mode & PF_X) > 0) * PTE_X) |
    (((elf_pt_mode & PF_W) > 0) * (PTE_W | PTE_R)) |
    (((elf_pt_mode & PF_R) > 0) * PTE_R)
  ;
}

int loadElf(elf_t* elf) {

  for (unsigned int i = 0; i < elf_getNumProgramHeaders(elf); i++) {
    if (elf_getProgramHeaderType(elf, i) != PT_LOAD) {
      continue;
    }

    uintptr_t start      = elf_getProgramHeaderVaddr(elf, i);
    uintptr_t file_end   = start + elf_getProgramHeaderFileSize(elf, i);
    uintptr_t memory_end = start + elf_getProgramHeaderMemorySize(elf, i);
    char* src            = (char*)(elf_getProgramSegment(elf, i));
    uintptr_t va         = start;
    int pt_mode          = pt_mode_from_elf(elf_getProgramHeaderFlags(elf, i));

    /* va is not page-aligned, so it doesn't own some of the page. Page may already be mapped. */
    if (RISCV_PAGE_OFFSET(va)) {
      if (RISCV_PAGE_OFFSET(va) != RISCV_PAGE_OFFSET((uintptr_t) src)) {
        printf("[loader] loadElf: va and src are misaligned");
        return -1;
      }
      uintptr_t new_page = allocPage(va, 0, pt_mode);
      if (!new_page)
        //return Error::PageAllocationFailure;asdf
        return -1; //TODO: error class laterasdf
      memcpy((void *) (new_page + RISCV_PAGE_OFFSET(va)), src, RISCV_PAGE_SIZE - RISCV_PAGE_OFFSET(va));
      // TODO: is it safe to free the page?
      va = PAGE_DOWN(va) + RISCV_PAGE_SIZE;
      src = (char *) (PAGE_DOWN((uintptr_t) src) + RISCV_PAGE_SIZE);
    }

    /* first load all pages that do not include .bss segment */
    while (va + RISCV_PAGE_SIZE <= file_end) {
      if (!mapPage(va, (uintptr_t)src, pt_mode))
        //return Error::PageAllocationFailure;
        return -1; //TODO: error class later
      src += RISCV_PAGE_SIZE;
      va += RISCV_PAGE_SIZE;
    }

    /* load the .bss segments */
    while (va < memory_end) {
      uintptr_t new_page = allocPage(va, 0, pt_mode);
      if (!new_page)
        //return Error::PageAllocationFailure;
        return -1; //TODO: error class later
      /* copy over non .bss part of the page if it's a part of the page */
      if (va < file_end) {
        memcpy((void*) new_page, src, file_end - va);
      }
      va += RISCV_PAGE_SIZE;
    }
  }

   //return Error::Success;
   return 0; //TODO: error class later
}

void
map_physical_memory(uintptr_t dram_base,
                    uintptr_t dram_size)
{
  uintptr_t ptr = EYRIE_LOAD_START;
  /* load address should not override kernel address */
  assert(RISCV_GET_PT_INDEX(ptr, 1) != RISCV_GET_PT_INDEX(RUNTIME_VA_START, 1));
  map_with_reserved_page_table(dram_base, dram_size,
      ptr, load_l2_page_table, load_l3_page_table);
}



int load_runtime(uintptr_t dummy,
                uintptr_t dram_base, uintptr_t dram_size, 
                uintptr_t runtime_base, uintptr_t user_base, 
                uintptr_t free_base, uintptr_t untrusted_ptr, 
                uintptr_t untrusted_size) {
  int ret = 0;

  // initialize free list
  initializeFreeList(free_base, dram_base, dram_size);

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
  ret = loadElf(&runtime_elf);
  if (ret < 0) {
    return ret;
  }

  // map enclave physical memory, so that runtime will be able to access all memory
  map_physical_memory(dram_base, dram_size);

  // map untrusted memory
  uintptr_t va        = EYRIE_UNTRUSTED_START;
  uintptr_t untr_iter = untrusted_ptr;
  while (va < EYRIE_UNTRUSTED_START + untrusted_size) {
    if (!mapPage(va, untr_iter, PTE_W | PTE_R)) {
      //return Error::PageAllocationFailure;
      return -1; //TODO: error class later
    }
    va += RISCV_PAGE_SIZE;
    untr_iter += RISCV_PAGE_SIZE;
  }

  return ret;
}

// TODO: exit instead, may need to set up some memory stuff first
void error_and_loop() {
  printf("[loader] ran into error. looping infinitely...\n");
  while(true) {
    printf(".");
  }
}

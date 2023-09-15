#include "loader/loader.h"
#include "loader/elf.h"
#include "string.h"
#include "mm/mm.h"
#include "mm/common.h"
#include "mm/vm_defs.h"
#include "mm/vm.h"

#define MIN(a,b) (((a)<(b))?(a):(b))

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

    /* va is not page-aligned, so it doesn't own some of the page. Page may already be mapped. */
    if (RISCV_PAGE_OFFSET(va)) {
      if (RISCV_PAGE_OFFSET(va) != RISCV_PAGE_OFFSET((uintptr_t) src)) {
        printf("[runtime] loadElf: va and src are misaligned");
        return -1;
      }
      uintptr_t new_page = alloc_page(vpn(va), PTE_U | PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V);
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
      uintptr_t src_pa = __pa((uintptr_t) src);
      if (!map_page(vpn(va), ppn(src_pa)))
        //return Error::PageAllocationFailure;
        return -1; //TODO: error class later
      src += RISCV_PAGE_SIZE;
      va += RISCV_PAGE_SIZE;
    }

    /* load the .bss segments */
    while (va < memory_end) {
      uintptr_t new_page = alloc_page(vpn(va), PTE_U | PTE_D | PTE_A | PTE_R | PTE_W | PTE_X | PTE_V);
      if (!new_page)
        //return Error::PageAllocationFailure;
        return -1; //TODO: error class later
      memset((void*) new_page, 0, MIN(RISCV_PAGE_SIZE, memory_end - va));
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

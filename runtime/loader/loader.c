#include "loader/loader.h"
#include "loader/elf.h"
#include "string.h"
#include "mm/mm.h"
#include "mm/common.h"
#include "mm/vm_defs.h"
#include "mm/vm.h"

int loadElf(elf_t* elf) {
  debug("Loading elf\n");

  for (unsigned int i = 0; i < elf_getNumProgramHeaders(elf); i++) {
    debug("loading %dth program header\n", i);
    if (elf_getProgramHeaderType(elf, i) != PT_LOAD) {
      continue;
    }

    uintptr_t start      = elf_getProgramHeaderVaddr(elf, i);
    uintptr_t file_end   = start + elf_getProgramHeaderFileSize(elf, i);
    uintptr_t memory_end = start + elf_getProgramHeaderMemorySize(elf, i);
    char* src            = (char*)(elf_getProgramSegment(elf, i));
    uintptr_t va         = start;

    debug("Loading initialized segment for program header %d\n", i);
    /* first load all pages that do not include .bss segment */
    while (va + RISCV_PAGE_SIZE <= file_end) {
      uintptr_t src_pa = __pa((uintptr_t) src);
      if (!map_page(vpn(va), ppn(src_pa)))
        //return Error::PageAllocationFailure;
        return -1; //TODO: error class later
      src += RISCV_PAGE_SIZE;
      va += RISCV_PAGE_SIZE;
    }

    /* next, load the page that has both initialized and uninitialized segments
     */
    if (va < file_end) {
      uintptr_t src_pa = __pa((uintptr_t) src);
      if (!map_page(vpn(va), ppn(src_pa)))
        //return Error::PageAllocationFailure;
        return -1; //TODO: error class later
      va += RISCV_PAGE_SIZE;
    }

    debug("Loading .bss segment for program header %d\n", i);
    /* finally, load the remaining .bss segments */
    while (va < memory_end) {
      uintptr_t new_page = alloc_page(vpn(va), PTE_R | PTE_W | PTE_U);
      if (!new_page)
        //return Error::PageAllocationFailure;
        return -1; //TODO: error class later
      memset((void*) new_page, 0, RISCV_PAGE_SIZE);
      va += RISCV_PAGE_SIZE;
    }
  }

   //return Error::Success;
   return 0; //TODO: error class later
}
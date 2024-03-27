#include "loader/loader.h"
#include "loader/elf.h"
#include "string.h"
#include "mm/mm.h"
#include "mm/common.h"
#include "mm/vm_defs.h"
#include "mm/vm.h"

resource_ptr_t* findIdentityResident(uintptr_t dram_base, const char* filename) {
  // note: SM validates all pointers and sizes are within epm
  enclave_bundle_header_t* ebundle_h = (enclave_bundle_header_t*) dram_base;
  resource_ptr_t* id_res_resource = (resource_ptr_t*) (dram_base + ebundle_h->id_res_arr);
  resource_ptr_t* id_abs_arr = (resource_ptr_t*) (dram_base + ebundle_h->id_abs_arr);
  for (; id_res_resource < id_abs_arr; id_res_resource++) {
    if (strcmp(id_res_resource->name, filename) == 0) {
      return id_res_resource;
    }
  }
  printf("findIdentityResident: filename \"%s\" not found\n", filename);
  return 0;
}

static inline int pt_mode_from_elf(int elf_pt_mode) {
  return 
    (((elf_pt_mode & PF_X) > 0) * PTE_X) |
    (((elf_pt_mode & PF_W) > 0) * (PTE_W | PTE_R | PTE_D)) |
    (((elf_pt_mode & PF_R) > 0) * PTE_R)
  ;
}

int loadElf(elf_t* elf, bool user) {
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
    pt_mode             |= (user > 0) * PTE_U;

    /* va is not page-aligned, so it doesn't own some of the page. Page may already be mapped. */
    if (RISCV_PAGE_OFFSET(va)) {
      if (RISCV_PAGE_OFFSET(va) != RISCV_PAGE_OFFSET((uintptr_t) src)) {
        printf("loadElf: va and src are misaligned\n");
        return -1;
      }
      uintptr_t new_page = alloc_page(vpn(va), pt_mode);
      if (!new_page)
        return -1;
      memcpy((void *) (new_page + RISCV_PAGE_OFFSET(va)), src, RISCV_PAGE_SIZE - RISCV_PAGE_OFFSET(va));
      va = PAGE_DOWN(va) + RISCV_PAGE_SIZE;
      src = (char *) (PAGE_DOWN((uintptr_t) src) + RISCV_PAGE_SIZE);
    }

    /* first load all pages that do not include .bss segment */
    while (va + RISCV_PAGE_SIZE <= file_end) {
      uintptr_t src_pa = __pa((uintptr_t) src);
      if (!map_page(vpn(va), ppn(src_pa), pt_mode))
        return -1;
      src += RISCV_PAGE_SIZE;
      va += RISCV_PAGE_SIZE;
    }

    /* load the .bss segments */
    while (va < memory_end) {
      uintptr_t new_page = alloc_page(vpn(va), pt_mode);
      if (!new_page)
        return -1;
      /* copy over non .bss part of the page if it's a part of the page */
      if (va < file_end) {
        memcpy((void*) new_page, src, file_end - va);
      }
      va += RISCV_PAGE_SIZE;
    }
  }

   return 0;
}

// assumes beginning and next file are page-aligned
static inline void freeUnusedElf(elf_t* elf) {
  assert(false); // TODO: needs free to be implemented properly
  for (unsigned int i = 0; i < elf_getNumProgramHeaders(elf); i++) {
    uintptr_t start      = elf_getProgramHeaderVaddr(elf, i);
    uintptr_t file_end   = start + elf_getProgramHeaderFileSize(elf, i);
    uintptr_t src        = (uintptr_t) elf_getProgramSegment(elf, i);

    if (elf_getProgramHeaderType(elf, i) != PT_LOAD) {
      uintptr_t src_end = file_end - start + src;
      for (; src < src_end; src += RISCV_PAGE_SIZE) {
        // free_page(vpn(src));
      }
      continue;
    }

    if (RISCV_PAGE_OFFSET(start)) {
      // free_page(vpn(start));
    }

    if (RISCV_PAGE_OFFSET(file_end)) {
      // free_page(vpn(file_end));
    }
  }
}

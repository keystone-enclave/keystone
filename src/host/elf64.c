/*
 * Copyright (c) 1999-2004 University of New South Wales
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <elf.h>
#include <elf64.h>
#include <inttypes.h>
#include <string.h>

/* ELF header functions */
int
elf64_checkFile(elf_t* elf) {
  if (sizeof(uintptr_t) != sizeof(uint64_t)) {
    return -1; /* not supported on 32-bit architecture */
  }

  if (elf->elfSize < sizeof(Elf64_Ehdr)) {
    return -1; /* file smaller than ELF header */
  }

  if (elf_check_magic((char*)elf->elfFile) < 0) {
    return -1; /* not an ELF file */
  }

  Elf64_Ehdr* header = (Elf64_Ehdr*)elf->elfFile;
  if (header->e_ident[EI_CLASS] != ELFCLASS64) {
    return -1; /* not a 64-bit ELF */
  }

  if (header->e_phentsize != sizeof(Elf64_Phdr)) {
    return -1; /* unexpected program header size */
  }

  if (header->e_shentsize != sizeof(Elf64_Shdr)) {
    return -1; /* unexpected section header size */
  }

  if (header->e_shstrndx >= header->e_shnum) {
    return -1; /* invalid section header string table section */
  }

  elf->elfClass = header->e_ident[EI_CLASS];
  return 0; /* elf header looks OK */
}

int
elf64_checkProgramHeaderTable(elf_t* elf) {
  Elf64_Ehdr* header = (Elf64_Ehdr*)elf->elfFile;
  size_t ph_end      = header->e_phoff + header->e_phentsize * header->e_phnum;
  if (elf->elfSize < ph_end || ph_end < header->e_phoff) {
    return -1; /* invalid program header table */
  }

  return 0;
}

int
elf64_checkSectionTable(elf_t* elf) {
  Elf64_Ehdr* header = (Elf64_Ehdr*)elf->elfFile;
  size_t sh_end      = header->e_shoff + header->e_shentsize * header->e_shnum;
  if (elf->elfSize < sh_end || sh_end < header->e_shoff) {
    return -1; /* invalid section header table */
  }

  return 0;
}

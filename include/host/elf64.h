/*
 * Copyright (c) 1999-2004 University of New South Wales
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#pragma once

#include <elf.h>
#include <stdint.h>

/* ELF header functions */
int
elf64_checkFile(elf_t* elf);

int
elf64_checkProgramHeaderTable(elf_t* elf);

int
elf64_checkSectionTable(elf_t* elf);

static inline bool
elf_isElf64(elf_t* elf) {
  return elf->elfClass == ELFCLASS64;
}

static inline Elf64_Ehdr
elf64_getHeader(elf_t* elf) {
  return *(Elf64_Ehdr*)elf->elfFile;
}

static inline uintptr_t
elf64_getEntryPoint(elf_t* file) {
  return elf64_getHeader(file).e_entry;
}

static inline Elf64_Phdr*
elf64_getProgramHeaderTable(elf_t* file) {
  return (Elf64_Phdr*)((uint8_t*)file->elfFile + elf64_getHeader(file).e_phoff);
}

static inline Elf64_Shdr*
elf64_getSectionTable(elf_t* file) {
  return (Elf64_Shdr*)((uint8_t*)file->elfFile + elf64_getHeader(file).e_shoff);
}

static inline size_t
elf64_getNumProgramHeaders(elf_t* file) {
  return elf64_getHeader(file).e_phnum;
}

static inline size_t
elf64_getNumSections(elf_t* elf) {
  return elf64_getHeader(elf).e_shnum;
}

static inline size_t
elf64_getSectionStringTableIndex(elf_t* elf) {
  return elf64_getHeader(elf).e_shstrndx;
}

/* Section header functions */
static inline size_t
elf64_getSectionNameOffset(elf_t* elf, size_t s) {
  return elf64_getSectionTable(elf)[s].sh_name;
}

static inline uint32_t
elf64_getSectionType(elf_t* file, size_t s) {
  return elf64_getSectionTable(file)[s].sh_type;
}

static inline size_t
elf64_getSectionFlags(elf_t* file, size_t s) {
  return elf64_getSectionTable(file)[s].sh_flags;
}

static inline uintptr_t
elf64_getSectionAddr(elf_t* elf, size_t i) {
  return elf64_getSectionTable(elf)[i].sh_addr;
}

static inline size_t
elf64_getSectionOffset(elf_t* elf, size_t i) {
  return elf64_getSectionTable(elf)[i].sh_offset;
}

static inline size_t
elf64_getSectionSize(elf_t* elf, size_t i) {
  return elf64_getSectionTable(elf)[i].sh_size;
}

static inline uint32_t
elf64_getSectionLink(elf_t* elf, size_t i) {
  return elf64_getSectionTable(elf)[i].sh_link;
}

static inline uint32_t
elf64_getSectionInfo(elf_t* elf, size_t i) {
  return elf64_getSectionTable(elf)[i].sh_info;
}

static inline size_t
elf64_getSectionAddrAlign(elf_t* elf, size_t i) {
  return elf64_getSectionTable(elf)[i].sh_addralign;
}

static inline size_t
elf64_getSectionEntrySize(elf_t* elf, size_t i) {
  return elf64_getSectionTable(elf)[i].sh_entsize;
}

/* Program header functions */
static inline uint32_t
elf64_getProgramHeaderType(elf_t* file, size_t ph) {
  return elf64_getProgramHeaderTable(file)[ph].p_type;
}

static inline size_t
elf64_getProgramHeaderOffset(elf_t* file, size_t ph) {
  return elf64_getProgramHeaderTable(file)[ph].p_offset;
}

static inline uintptr_t
elf64_getProgramHeaderVaddr(elf_t* file, size_t ph) {
  return elf64_getProgramHeaderTable(file)[ph].p_vaddr;
}

static inline uintptr_t
elf64_getProgramHeaderPaddr(elf_t* file, size_t ph) {
  return elf64_getProgramHeaderTable(file)[ph].p_paddr;
}

static inline size_t
elf64_getProgramHeaderFileSize(elf_t* file, size_t ph) {
  return elf64_getProgramHeaderTable(file)[ph].p_filesz;
}

static inline size_t
elf64_getProgramHeaderMemorySize(elf_t* file, size_t ph) {
  return elf64_getProgramHeaderTable(file)[ph].p_memsz;
}

static inline uint32_t
elf64_getProgramHeaderFlags(elf_t* file, size_t ph) {
  return elf64_getProgramHeaderTable(file)[ph].p_flags;
}

static inline size_t
elf64_getProgramHeaderAlign(elf_t* file, size_t ph) {
  return elf64_getProgramHeaderTable(file)[ph].p_align;
}

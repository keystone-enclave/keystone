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
elf32_checkFile(elf_t* elf);

int
elf32_checkProgramHeaderTable(elf_t* elf);

int
elf32_checkSectionTable(elf_t* elf);

static inline bool
elf_isElf32(elf_t* elf) {
  return elf->elfClass == ELFCLASS32;
}

static inline Elf32_Ehdr
elf32_getHeader(elf_t* elf) {
  return *(Elf32_Ehdr*)elf->elfFile;
}

static inline uintptr_t
elf32_getEntryPoint(elf_t* elf) {
  return elf32_getHeader(elf).e_entry;
}

static inline Elf32_Phdr*
elf32_getProgramHeaderTable(elf_t* file) {
  return (Elf32_Phdr*)((uint8_t*)file->elfFile + elf32_getHeader(file).e_phoff);
}

static inline Elf32_Shdr*
elf32_getSectionTable(elf_t* elf) {
  return (Elf32_Shdr*)((uint8_t*)elf->elfFile + elf32_getHeader(elf).e_shoff);
}

static inline size_t
elf32_getNumProgramHeaders(elf_t* elf) {
  return elf32_getHeader(elf).e_phnum;
}

static inline size_t
elf32_getNumSections(elf_t* elf) {
  return elf32_getHeader(elf).e_shnum;
}

static inline size_t
elf32_getSectionStringTableIndex(elf_t* elf) {
  return elf32_getHeader(elf).e_shstrndx;
}

/* Section header functions */
static inline size_t
elf32_getSectionNameOffset(elf_t* elf, size_t s) {
  return elf32_getSectionTable(elf)[s].sh_name;
}

static inline uint32_t
elf32_getSectionType(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_type;
}

static inline size_t
elf32_getSectionFlags(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_flags;
}

static inline uintptr_t
elf32_getSectionAddr(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_addr;
}

static inline size_t
elf32_getSectionOffset(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_offset;
}

static inline size_t
elf32_getSectionSize(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_size;
}

static inline uint32_t
elf32_getSectionLink(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_link;
}

static inline uint32_t
elf32_getSectionInfo(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_info;
}

static inline size_t
elf32_getSectionAddrAlign(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_addralign;
}

static inline size_t
elf32_getSectionEntrySize(elf_t* elf, size_t i) {
  return elf32_getSectionTable(elf)[i].sh_entsize;
}

/* Program header functions */
static inline uint32_t
elf32_getProgramHeaderType(elf_t* file, size_t ph) {
  return elf32_getProgramHeaderTable(file)[ph].p_type;
}

static inline size_t
elf32_getProgramHeaderOffset(elf_t* file, size_t ph) {
  return elf32_getProgramHeaderTable(file)[ph].p_offset;
}

static inline uintptr_t
elf32_getProgramHeaderVaddr(elf_t* file, size_t ph) {
  return elf32_getProgramHeaderTable(file)[ph].p_vaddr;
}

static inline uintptr_t
elf32_getProgramHeaderPaddr(elf_t* file, size_t ph) {
  return elf32_getProgramHeaderTable(file)[ph].p_paddr;
}

static inline size_t
elf32_getProgramHeaderFileSize(elf_t* file, size_t ph) {
  return elf32_getProgramHeaderTable(file)[ph].p_filesz;
}

static inline size_t
elf32_getProgramHeaderMemorySize(elf_t* file, size_t ph) {
  return elf32_getProgramHeaderTable(file)[ph].p_memsz;
}

static inline uint32_t
elf32_getProgramHeaderFlags(elf_t* file, size_t ph) {
  return elf32_getProgramHeaderTable(file)[ph].p_flags;
}

static inline size_t
elf32_getProgramHeaderAlign(elf_t* file, size_t ph) {
  return elf32_getProgramHeaderTable(file)[ph].p_align;
}

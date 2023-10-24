#pragma once
#include <stddef.h>
#include <stdint.h>
#include "vm_defs.h"

extern uintptr_t freeList; 
extern uintptr_t epmBase; 
extern size_t epmSize;

uint32_t mapPage(uintptr_t va, uintptr_t pa);
uint32_t allocPage(uintptr_t va, uintptr_t src);
size_t epmAllocVspace(uintptr_t addr, size_t num_pages); // TODO: unused
uintptr_t epm_va_to_pa(uintptr_t addr); // TODO: unused

void map_with_reserved_page_table(uintptr_t base, uintptr_t size, uintptr_t ptr, pte* l2_pt, pte* l3_pt);

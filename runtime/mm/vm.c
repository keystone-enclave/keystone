#include <stddef.h>
#include <stdint.h>
#include "mm/vm.h"

uintptr_t runtime_va_start;

/* root page table */
pte* root_page_table;

#ifdef LOADER_BIN

/* no-ops */

uintptr_t kernel_va_to_pa(void* ptr)
{
  return (uintptr_t) ptr;
}

uintptr_t __va(uintptr_t pa)
{
  return pa;
}

uintptr_t __pa(uintptr_t va)
{
  return va;
}

#else // !LOADER_BIN

uintptr_t kernel_va_to_pa(void* ptr)
{
  return (uintptr_t) ptr - kernel_offset;
}

uintptr_t __va(uintptr_t pa)
{
  return (pa - load_pa_start) + EYRIE_LOAD_START;
}

uintptr_t __pa(uintptr_t va)
{
  return (va - EYRIE_LOAD_START) + load_pa_start;
}

/* Program break */
uintptr_t program_break;

/* freemem */
uintptr_t freemem_va_start;
size_t freemem_size;

/* shared buffer */
uintptr_t shared_buffer;
uintptr_t shared_buffer_size;

uintptr_t kernel_offset;
uintptr_t load_pa_start;

#endif // LOADER_BIN



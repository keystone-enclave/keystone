#ifndef _MM_H_
#define _MM_H_
#include <stdint.h>
#include <stddef.h>

uintptr_t translate(uintptr_t va);

#ifdef USE_FREEMEM
uintptr_t remap_physical_page(uintptr_t vpn, uintptr_t ppn, int flags);
size_t remap_physical_pages(uintptr_t vpn, uintptr_t ppn, size_t count, int flags);

uintptr_t alloc_page(uintptr_t vpn, int flags);
void free_page(uintptr_t vpn);
size_t alloc_pages(uintptr_t vpn, size_t count, int flags);
void free_pages(uintptr_t vpn, size_t count);
size_t test_va_range(uintptr_t vpn, size_t count);

uintptr_t get_program_break();
void set_program_break(uintptr_t new_break);
#endif /* USE_FREEMEM */

#endif /* _MM_H_ */

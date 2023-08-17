#include "mm/vm.h"

uintptr_t runtime_va_start;
uintptr_t kernel_offset;
uintptr_t load_pa_start;
pte* root_page_table;

#ifdef USE_FREEMEM

/* Program break */
uintptr_t program_break;

/* freemem */
uintptr_t freemem_va_start;
size_t freemem_size;
#endif // USE_FREEMEM

/* shared buffer */
uintptr_t shared_buffer;
uintptr_t shared_buffer_size;



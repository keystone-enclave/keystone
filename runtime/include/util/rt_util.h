#ifndef _RT_UTIL_H_
#define _RT_UTIL_H_

#include <stddef.h>

#include "util/regs.h"
#include "mm/vm_defs.h"

#define FATAL_DEBUG

size_t rt_util_getrandom(void* vaddr, size_t buflen);
void not_implemented_fatal(struct encl_ctx* ctx);
void rt_util_misc_fatal();
void rt_page_fault(struct encl_ctx* ctx);
void tlb_flush(void);

extern unsigned char rt_copy_buffer_1[RISCV_PAGE_SIZE];
extern unsigned char rt_copy_buffer_2[RISCV_PAGE_SIZE];

#endif /* _RT_UTIL_H_ */

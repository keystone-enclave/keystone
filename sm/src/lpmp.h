#ifndef _LPMP_H_
#define _LPMP_H_

void host_regions_init(void);

int host_add_region(uintptr_t pa, size_t size, int is_inst);

void host_split_region(uintptr_t pa, size_t size, int is_inst);

void activate_host_lpmp(void);

void pmp_clear(void);

int pmp_fault_handler(ulong mtval);

#endif

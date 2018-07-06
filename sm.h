#ifndef sm_h
#define sm_h

//FIXME: just arbitrary 128MB range
#include <stdint.h>
#include "pmp.h"
#include "sm-sbi.h"
#include "encoding.h"

int sm_region_init(uintptr_t addr, uint64_t size, uint8_t perm);

void sm_init(void);

#endif

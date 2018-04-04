#ifndef sm_h
#define sm_h

//FIXME: just arbitrary 128MB range
#define SM_PMP_START	0x50000000
#define SM_PMP_END		0x58000000

#include <stdint.h>
#include "encoding.h"
void sm_pmp_region_init(uintptr_t start, uintptr_t end);

void sm_init(void);

#endif

#ifndef sm_h
#define sm_h

//FIXME: just arbitrary 128MB range
#define N_PMP				16
#include <stdint.h>
#include "sm-sbi.h"
#include "encoding.h"

int sm_region_init(uintptr_t addr, uint64_t size, uint8_t perm);

void sm_init(void);

#define ASM_PMP_CONFIG(n, g, addr, pmpc) \
{	\
	asm volatile ("la t0, 1f\n\t" \
								"csrrw t0, mtvec, t0\n\t" \
								"csrw pmpaddr"#n", %0\n\t" \
								"csrw pmpcfg"#g", %1\n\t" \
								".align 2\n\t" \
								"1: csrw mtvec, t0" \
								: : "r" (addr), "r" (pmpc) : "t0"); \
}
#endif

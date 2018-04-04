#include "sm.h"
void sm_pmp_region_init(uintptr_t start, uintptr_t end)
{
	uintptr_t pmpc = (PMP_TOR| PMP_R|PMP_W|PMP_X) << 16 | (PMP_L|PMP_TOR) << 8 | (PMP_TOR | PMP_R | PMP_W | PMP_X); 
	asm volatile ("la t0, 1f\n\t"
								"csrrw t0, mtvec, t0\n\t"
								"csrw pmpaddr0, %1\n\t"
								"csrw pmpaddr1, %2 \n\t"
								"csrw pmpaddr2, %3 \n\t"
								"csrw pmpcfg0, %0 \n\t"
								".align 2\n\t"
								"1: csrw mtvec, t0"
								: : "r" (pmpc), "r" (start>>2), "r" (end>>2), "r" (-1UL) : "t0");
}

void sm_init(void)
{
	//set experimental 128MB
	sm_pmp_region_init(SM_PMP_START, SM_PMP_END);
}

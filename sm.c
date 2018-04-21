#include "sm.h"
#include "mtrap.h"

typedef unsigned char byte;
int32_t region_bitmap = 0;
uint8_t pmpc[N_PMP>>2] = {0,};
uintptr_t pmpa[N_PMP] = {0,};

extern byte sanctum_sm_signature[64];
extern byte sanctum_dev_public_key[32];

extern unsigned int sanctum_sm_size[1];
#define LIST_OF_PMP_REGS	X(0,0)  X(1,0)  X(2,0)  X(3,0) \
 													X(4,1)  X(5,1)  X(6,1)  X(7,1) \
													X(8,2)  X(9,2)  X(10,2) X(11,2) \
													X(12,3) X(13,3) X(14,3) X(15,3)

void pmp_config(int n, int g, uintptr_t addr, int8_t pmpc)
{
	switch(n) {
#define X(n,g) case n: { ASM_PMP_CONFIG(n, g, addr, pmpc); return; }
	LIST_OF_PMP_REGS
#undef X
	}
}

int sm_region_init(uintptr_t start, uint64_t size, uint8_t perm)
{
	// do not allow over 256 MB
	if(size > 0x10000000)
		return -1;

	// size should be power of 2
	if(!(size && !(size&(size-1))))
		return -1;

	//find avaiable pmp idx
	int region_idx;
	for(region_idx=0; region_idx<N_PMP; region_idx++)
	{
		if(! (region_bitmap & (0x1 << region_idx)))
			break;
	}
	//printm("region_idx: %d, region_cfg: %d\n", region_idx, region_idx >> 2);
	pmpc[region_idx>>2] |= ((PMP_NAPOT | perm)  << (8*(region_idx%4)));
	pmpa[region_idx] = (start|(size-1))>>3;	

	pmp_config(region_idx, region_idx>>2, pmpa[region_idx], pmpc[region_idx>>2]);

	//printm("%x (%x)\n", pmpa[region_idx], pmpc[region_idx>>2]);	
	region_bitmap |= (0x1 << region_idx);

	//printm("region_idx: %d, bitmap: 0x%x\n", region_idx, region_bitmap);

	return region_idx;
}

void sm_set_mtvec()
{
	
}

void sm_print_cert()
{
	int i;

	printm("Booting from Security Monitor\n");
	printm("Size: %d\n", sanctum_sm_size[0]);

	printm("============ PUBKEY =============\n");
	for(i=0; i<8; i+=1)
	{
		printm("%x",*((int*)sanctum_dev_public_key+i));
		if(i%4==3) printm("\n");
	}	
	printm("=================================\n");
	
	printm("=========== SIGNATURE ===========\n");
	for(i=0; i<16; i+=1)
	{
		printm("%x",*((int*)sanctum_sm_signature+i));
		if(i%4==3) printm("\n");
	}
	printm("=================================\n");
}

void sm_init(void)
{
	//set PMP region
	
	sm_region_init(0x80000000, 0x200000, 0);
	//sm_region_init(0, (-1UL), PMP_R | PMP_X | PMP_W);
	sm_set_mtvec();

	sm_print_cert();
}

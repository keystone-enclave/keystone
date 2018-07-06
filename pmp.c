#include "pmp.h"

int32_t reg_bitmap = 0;
int32_t region_def_bitmap = 0;
uintptr_t pmpcfg[PMP_N_REG>>2] = {0,};
uintptr_t pmpaddr[PMP_N_REG] = {0,};

struct pmp_region regions[PMP_MAX_N_REGION];

#define LIST_OF_PMP_REGS	X(0,0)  X(1,0)  X(2,0)  X(3,0) \
 													X(4,1)  X(5,1)  X(6,1)  X(7,1) \
													X(8,2)  X(9,2)  X(10,2) X(11,2) \
													X(12,3) X(13,3) X(14,3) X(15,3)

#define PMP_SET(n, g, addr, pmpc) \
{ uintptr_t oldcfg = read_csr(pmpcfg##g); \
  pmpc |= (oldcfg & ~(0xff << 8*(n%4))); \
	asm volatile ("la t0, 1f\n\t" \
								"csrrw t0, mtvec, t0\n\t" \
								"csrw pmpaddr"#n", %0\n\t" \
								"csrw pmpcfg"#g", %1\n\t" \
								".align 2\n\t" \
								"1: csrw mtvec, t0" \
								: : "r" (addr), "r" (pmpc) : "t0"); \
}

#define PMP_UNSET(n, g) \
{ uintptr_t pmpc = read_csr(pmpcfg##g); \
  pmpc &= ~(0xff << 8*(n%4)); \
  asm volatile ("la t0, 1f \n\t" \
                "csrrw t0, mtvec, t0 \n\t" \
                "csrw pmpaddr"#n", %0\n\t" \
                "csrw pmpcfg"#g", %1\n\t" \
                ".align 2\n\t" \
                "1: csrw mtvec, t0" \
                : : "r" (0), "r" (pmpc) : "t0"); \
}

int pmp_set(int region_idx)
{
  if(!(region_def_bitmap & (0x1 << region_idx)))
  {
    printm("pmp_set(): Invalid PMP region index\n");
    return -1;
  }
  if(regions[region_idx].reg_idx >= 0)
  {
    printm("pmp_set(): PMP region already set\n"); 
    return -1;
  }

  int reg_idx;
  for(reg_idx=0; reg_idx < PMP_N_REG; reg_idx++)
  {
    if(!(reg_bitmap & (0x1 << reg_idx)))
      break;
  }
  if(reg_idx == PMP_N_REG)
  {
    printm("pmp_set(): No available PMP register\n");
    return -1;
  }

  uintptr_t pmpcfg = (uintptr_t) regions[region_idx].cfg << (8*(reg_idx%4));
  uintptr_t pmpaddr = regions[region_idx].addr;

  printm("pmp_set(): reg %d, pmpcfg:%x, pmpaddr:%x\n", reg_idx, pmpcfg, pmpaddr);

	//printm("%x (%x)\n", pmpa[reg_idx], pmpc[reg_idx>>2]);	
	reg_bitmap |= (0x1 << reg_idx);
	regions[region_idx].reg_idx = reg_idx;
  //printm("reg_idx: %d, bitmap: 0x%x\n", reg_idx, reg_bitmap);

  int n=reg_idx, g=reg_idx>>2;
  switch(n) {
#define X(n,g) case n: { PMP_SET(n, g, pmpaddr, pmpcfg); return 0; }
	LIST_OF_PMP_REGS
#undef X
  }
  return -1;
}

int pmp_unset(int region_idx)
{
  if(!(region_def_bitmap & (0x1 << region_idx)))
  {
    printm("pmp_unset(): Invalid PMP region index\n");
    return -1;
  }
  int reg_idx = regions[region_idx].reg_idx;
  if(reg_idx < 0)
  {
    printm("pmp_unset(): PMP region not set\n");
    return -1;
  }

  int n=reg_idx, g=reg_idx>>2;
  switch(n) {
#define X(n,g) case n: { PMP_UNSET(n, g); break;}
  LIST_OF_PMP_REGS
#undef X
    default:
      return -1;
  }

  reg_bitmap &= ~(0x1 << reg_idx);
  regions[region_idx].reg_idx = -1;
  return 0;
}

void pmp_region_debug_print(int region_idx)
{
  if(!(region_def_bitmap & (0x01 << region_idx)))
  {
    printm("pmp_region_debug_print(): Invalid PMP region index\n");
    return;
  }
  uintptr_t start = regions[region_idx].start;
  uint64_t size = regions[region_idx].size;
  uint8_t perm = regions[region_idx].perm;
  uint8_t cfg = regions[region_idx].cfg;
  uintptr_t addr = regions[region_idx].addr;
  int reg_idx = regions[region_idx].reg_idx;

  printm("start: 0x%llx\n"
    "size: 0x%llx (%d)\n"
    "perm: 0x%x\n"
    "cfg: 0x%x\n"
    "addr<<3: 0x%llx\n"
    "reg_idx: %d\n"
   , start, size, size, perm, cfg, addr<<3, reg_idx ); 
  
}

int pmp_region_init(uintptr_t start, uint64_t size, uint8_t perm)
{
	// do not allow over 256 MB
	if(size > PMP_MAX_SIZE)
		return -1;

	// size should be power of 2
	if(!(size && !(size&(size-1))))
		return -1;
  
	//find avaiable pmp region idx
  int region_idx;
  for(region_idx=0; region_idx < PMP_MAX_N_REGION; region_idx++)
  {
    if(! (region_def_bitmap & (0x1 << region_idx)))
      break;
  }
  
  if(region_idx == PMP_MAX_N_REGION)
  {
    printm("Reached the maximum number of PMP regions\n");
    return -1;
  }

  // initialize the region (only supports NAPOT)
  regions[region_idx].start = start;
  regions[region_idx].size = size;
  regions[region_idx].perm = perm;
  regions[region_idx].cfg = (PMP_NAPOT | perm);
  regions[region_idx].addr = (start | (size-1)) >> 3;
  regions[region_idx].reg_idx = -1;
  region_def_bitmap |= (0x1 << region_idx);

  return region_idx;
}

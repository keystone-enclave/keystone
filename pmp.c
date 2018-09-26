#include "pmp.h"

uint32_t reg_bitmap = 0;
uint32_t region_def_bitmap = 0;

struct pmp_region regions[PMP_MAX_N_REGION];

#if __riscv_xlen == 64
# define LIST_OF_PMP_REGS  X(0,0)  X(1,0)  X(2,0)  X(3,0) \
                           X(4,0)  X(5,0)  X(6,0)  X(7,0) \
                           X(8,2)  X(9,2)  X(10,2) X(11,2) \
                          X(12,2) X(13,2) X(14,2) X(15,2)
# define PMP_PER_GROUP  8
#else
# define LIST_OF_PMP_REGS  X(0,0)  X(1,0)  X(2,0)  X(3,0) \
                           X(4,1)  X(5,1)  X(6,1)  X(7,1) \
                           X(8,2)  X(9,2)  X(10,2) X(11,2) \
                           X(12,3) X(13,3) X(14,3) X(15,3)
# define PMP_PER_GROUP  4
#endif

#define PMP_SET(n, g, addr, pmpc) \
{ uintptr_t oldcfg = read_csr(pmpcfg##g); \
  pmpc |= (oldcfg & ~((uintptr_t)0xff << 8*(n%PMP_PER_GROUP))); \
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
  pmpc &= ~((uintptr_t)0xff << 8*(n%PMP_PER_GROUP)); \
  asm volatile ("la t0, 1f \n\t" \
                "csrrw t0, mtvec, t0 \n\t" \
                "csrw pmpaddr"#n", %0\n\t" \
                "csrw pmpcfg"#g", %1\n\t" \
                ".align 2\n\t" \
                "1: csrw mtvec, t0" \
                : : "r" (0), "r" (pmpc) : "t0"); \
}

#define PMP_ERROR(error, msg) {\
  printm("%s:" msg "\n", __func__);\
  return error; \
}

inline int is_pmp_region_set(int region_idx) {
  return (regions[region_idx].reg_idx >= 0);
}
inline int is_pmp_region_valid(int region_idx) {
  return TEST_BIT(region_def_bitmap, region_idx);
}
inline int is_pmp_reg_set(int reg_idx) {
  return TEST_BIT(reg_bitmap, reg_idx);
}
int search_rightmost_unset(uint32_t bitmap, int max) {
  int i;
  uint32_t mask = 0x1;
  for(i=0; i<max; i++) {
    if( ~bitmap & mask )
      return i;
    mask = mask << 1;
  }
  return -1;
}

int get_free_region_idx() {
  return search_rightmost_unset(region_def_bitmap, PMP_MAX_N_REGION);
}
int get_free_reg_idx() {
  return search_rightmost_unset(reg_bitmap, PMP_N_REG);
}

int pmp_set_bind_reg(int region_idx, int reg_idx)
{ 
  if(!is_pmp_region_valid(region_idx)) 
    PMP_ERROR(-EINVAL, "Invalid PMP region index");
  
  if(is_pmp_region_set(region_idx))
    PMP_ERROR(-EINVAL, "PMP region already set");

  uintptr_t pmpcfg = (uintptr_t) regions[region_idx].cfg << (8*(reg_idx%PMP_PER_GROUP));
  uintptr_t pmpaddr = regions[region_idx].addr;

  //printm("pmp_set(): reg %d, pmpcfg:%lx, pmpaddr<<2:%lx\n", reg_idx, pmpcfg, pmpaddr<<2);

  SET_BIT(reg_bitmap, reg_idx);
  regions[region_idx].reg_idx = reg_idx;

  int n=reg_idx, g=reg_idx>>2;
  switch(n) {
#define X(n,g) case n: { PMP_SET(n, g, pmpaddr, pmpcfg); return 0; }
  LIST_OF_PMP_REGS
#undef X
  } 
  // NEVER reach here
  // bbl is not properly configured.
  PMP_ERROR(-EFAULT, "Unreachable code");
}

int pmp_set(int region_idx)
{ 
  int reg_idx = get_free_reg_idx();
  if(reg_idx < 0) 
    PMP_ERROR(-EBUSY, "No available PMP register");

  return pmp_set_bind_reg(region_idx, reg_idx);
}

int pmp_unset(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(-EINVAL,"Invalid PMP region index");

  if(!is_pmp_region_set(region_idx))
    PMP_ERROR(-EINVAL, "PMP region not set");
  
  int reg_idx = regions[region_idx].reg_idx;

  int n=reg_idx, g=reg_idx>>2;
  switch(n) {
#define X(n,g) case n: { PMP_UNSET(n, g); break;}
  LIST_OF_PMP_REGS
#undef X
    default:
      return -1;
  }

  UNSET_BIT(reg_bitmap, reg_idx);
  regions[region_idx].reg_idx = -1;
  return 0;
}

int pmp_region_debug_print(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(-EINVAL,"Invalid PMP region index");
  
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

  return 0;  
}

int pmp_region_init(uintptr_t start, uint64_t size, uint8_t perm)
{
  // do not allow over 256 MB
  if(size > PMP_MAX_SIZE)
    PMP_ERROR(-ENOSYS, "PMP size over 256MB not implemented");

  // size should be power of 2
  if(!(size && !(size&(size-1))))
    PMP_ERROR(-EINVAL, "PMP size should be power of 2");
  
  //find avaiable pmp region idx
  int region_idx = get_free_region_idx();
  if(region_idx < 0)
    PMP_ERROR(-EFAULT, "Reached the maximum number of PMP regions");

  // initialize the region (only supports NAPOT)
  regions[region_idx].start = start;
  regions[region_idx].size = size;
  regions[region_idx].perm = perm;
  regions[region_idx].cfg = (PMP_NAPOT | perm);
  regions[region_idx].addr = (start | (size/2-1)) >> 2;
  regions[region_idx].reg_idx = -1;
  SET_BIT(region_def_bitmap, region_idx);

  return region_idx;
}

int pmp_region_free(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(-EINVAL, "Invalid PMP region index");
  
  if(is_pmp_region_set(region_idx)) {
    int ret = pmp_unset(region_idx);
    if(ret) {
      return ret;
    }
  }

  regions[region_idx].start = 0;
  regions[region_idx].size = 0;
  regions[region_idx].perm = 0;
  regions[region_idx].cfg = 0;
  regions[region_idx].addr = 0;
  UNSET_BIT(region_def_bitmap, region_idx);
  
  return 0;
}

int set_os_pmp_region()
{
  int region_idx = get_free_region_idx();
  int reg_idx = PMP_N_REG - 1; //last PMP reg
  uint8_t perm = PMP_W | PMP_X | PMP_R; 
  regions[region_idx].start = 0;
  regions[region_idx].size = -1UL;
  regions[region_idx].perm = perm;
  regions[region_idx].cfg = (PMP_NAPOT | perm);
  regions[region_idx].addr = (-1UL);
  regions[region_idx].reg_idx = -1;
  SET_BIT(region_def_bitmap, region_idx);

  return pmp_set_bind_reg(region_idx, reg_idx);
}

void* pmp_get_addr(int region_idx)
{
  if(!TEST_BIT(region_def_bitmap, region_idx))
  {
    printm("pmp_get_addr(): Invalid PMP region index\n");
    return NULL;
  }
  
  return (void*)regions[region_idx].start;
}

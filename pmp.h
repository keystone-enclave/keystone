#ifndef _PMP_H_
#define _PMP_H_

#include "sm.h"
#include "mtrap.h"
#include <errno.h>

#define PMP_N_REG         16 //number of PMP registers
#define PMP_MAX_N_REGION  PMP_N_REG //maximum number of PMP regions

#define SET_BIT(bitmap, n) (bitmap |= (0x1 << n))
#define UNSET_BIT(bitmap, n) (bitmap &= ~(0x1 << n))
#define TEST_BIT(bitmap, n) (bitmap & (0x1 << n))

enum pmp_priority {
  PMP_PRI_ANY,
  PMP_PRI_TOP,
  PMP_PRI_BOTTOM,
};

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
  pmpc |= (oldcfg & ~((uintptr_t)0xff << (uintptr_t)8*(n%PMP_PER_GROUP))); \
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
  pmpc &= ~((uintptr_t)0xff << (uintptr_t)8*(n%PMP_PER_GROUP)); \
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

int pmp_region_debug_print(int region);
int pmp_region_init_atomic(uintptr_t start, uint64_t size, uint8_t perm, enum pmp_priority pri, int* rid);
int pmp_region_init(uintptr_t start, uint64_t size, uint8_t perm, enum pmp_priority pri, int* rid);
int pmp_region_free_atomic(int region);
int pmp_set(int n);
int pmp_set_global(int n);
int pmp_unset(int n);
int pmp_unset_global(int n);
void* pmp_get_addr(int region);
uint64_t pmp_get_size(int region);

struct pmp_region
{
  uintptr_t start;
  uint64_t size;
  uint8_t perm;
  uint8_t cfg;
  uintptr_t addr;
  int reg_idx;
};
#endif

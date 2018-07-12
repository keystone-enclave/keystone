#ifndef _PMP_H_
#define _PMP_H_

#include "sm.h"
#include "mtrap.h"
#include <errno.h>

#define PMP_N_REG         16 //number of PMP registers
#define PMP_MAX_N_REGION  PMP_N_REG //maximum number of PMP regions
#define PMP_MAX_SIZE      0x10000000 //maximum size of a PMP region

#define SET_BIT(bitmap, n) (bitmap |= (0x1 << n))
#define UNSET_BIT(bitmap, n) (bitmap &= ~(0x1 << n))
#define TEST_BIT(bitmap, n) (bitmap & (0x1 << n))

void pmp_region_debug_print(int region);
int pmp_region_init(uintptr_t start, uint64_t size, uint8_t perm);
int pmp_set(int n);
int pmp_unset(int n);

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

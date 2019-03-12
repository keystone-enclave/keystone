//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _PMP_H_
#define _PMP_H_

#include "sm.h"
#include "mtrap.h"
#include <errno.h>

#define PMP_N_REG         8 //number of PMP registers
#define PMP_MAX_N_REGION  16 //maximum number of PMP regions

#define SET_BIT(bitmap, n) (bitmap |= (0x1 << (n)))
#define UNSET_BIT(bitmap, n) (bitmap &= ~(0x1 << (n)))
#define TEST_BIT(bitmap, n) (bitmap & (0x1 << (n)))

enum pmp_priority {
  PMP_PRI_ANY,
  PMP_PRI_TOP,
  PMP_PRI_BOTTOM,
};

#define PMP_ALL_PERM  (PMP_W | PMP_X | PMP_R)
#define PMP_NO_PERM   0

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
                "sfence.vma\n\t"\
                ".align 2\n\t" \
                "1: csrw mtvec, t0 \n\t" \
                : : "r" (addr), "r" (pmpc) : "t0"); \
}

#define PMP_UNSET(n, g) \
{ uintptr_t pmpc = read_csr(pmpcfg##g); \
  pmpc &= ~((uintptr_t)0xff << (uintptr_t)8*(n%PMP_PER_GROUP)); \
  asm volatile ("la t0, 1f \n\t" \
                "csrrw t0, mtvec, t0 \n\t" \
                "csrw pmpaddr"#n", %0\n\t" \
                "csrw pmpcfg"#g", %1\n\t" \
                "sfence.vma\n\t"\
                ".align 2\n\t" \
                "1: csrw mtvec, t0" \
                : : "r" (0), "r" (pmpc) : "t0"); \
}

#define PMP_ERROR(error, msg) {\
  printm("%s:" msg "\n", __func__);\
  return error; \
}


typedef struct
{
  uint64_t size;
  uint8_t addrmode;
  uintptr_t addr;
  int allow_overlap;
  int reg_idx;
} region_t;

typedef int region_id_t;
static region_t regions[PMP_MAX_N_REGION];

int pmp_region_init_atomic(uintptr_t start, uint64_t size, enum pmp_priority pri, region_id_t* rid, int allow_overlap);
int pmp_region_init(uintptr_t start, uint64_t size, enum pmp_priority pri, region_id_t* rid, int allow_overlap);
int pmp_region_free_atomic(region_id_t region);
int pmp_set(region_id_t n, uint8_t perm);
int pmp_set_global(region_id_t n, uint8_t perm);
int pmp_unset(region_id_t n);
int pmp_unset_global(region_id_t n);

int detect_region_overlap_atomic(uintptr_t base, uintptr_t size);

////////////////////////////////
/* PMP region_t interfaces */
////////////////////////////////
static inline int region_register_idx(region_id_t i)
{
  return regions[i].reg_idx;
}
static inline int region_allows_overlap(region_id_t i)
{
  return regions[i].allow_overlap;
}
static inline uintptr_t region_get_addr(region_id_t i)
{
  return regions[i].addr;
}
static inline uint64_t region_get_size(region_id_t i)
{
  return regions[i].size;
}
static inline int region_is_napot(region_id_t i)
{
  return regions[i].addrmode == PMP_NAPOT;
}
static inline int region_is_tor(region_id_t i)
{
  return regions[i].addrmode == PMP_TOR;
}
static inline int region_needs_two_entries(region_id_t i)
{
  return region_is_tor(i) && regions[i].reg_idx > 0;
}
static inline int region_is_napot_all(region_id_t i)
{
  return regions[i].addr == 0 && regions[i].size == -1UL;
}
static inline uintptr_t region_pmpaddr_val(region_id_t i)
{
  if(region_is_napot_all(i))
    return (-1UL);
  else if(region_is_napot(i))
    return (regions[i].addr | (regions[i].size/2-1)) >> 2;
  else if(region_is_tor(i))
    return (regions[i].addr + regions[i].size) >> 2;
  else
    return 0;
}
static inline uintptr_t region_pmpcfg_val(region_id_t i, int reg_idx, uint8_t perm_bits)
{
  return (uintptr_t) (regions[i].addrmode | perm_bits) << (8*(reg_idx%PMP_PER_GROUP));
}
static void region_clear_all(region_id_t i)
{
  regions[i].addr = 0;
  regions[i].size = 0;
  regions[i].addrmode = 0;
  regions[i].allow_overlap = 0;
  regions[i].reg_idx = 0;
}
static void region_init(region_id_t i,
                        uintptr_t addr,
                        uint64_t size,
                        uint8_t addrmode,
                        int allow_overlap,
                        int reg_idx)
{
  regions[i].addr = addr;
  regions[i].size = size;
  regions[i].addrmode = addrmode;
  regions[i].allow_overlap = allow_overlap;
  regions[i].reg_idx = (addrmode == PMP_TOR && reg_idx > 0 ? reg_idx + 1 : reg_idx);
}

#endif

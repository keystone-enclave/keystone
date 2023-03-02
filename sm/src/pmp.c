//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "sm_assert.h"
#include "pmp.h"
#include "cpu.h"
#include "safe_math_util.h"
#include "sm-sbi-opensbi.h"
#include "page.h"
#include "ipi.h"
#include <sbi/sbi_hart.h>
#include <sbi/riscv_asm.h>
#include <sbi/riscv_locks.h>
#include <sbi/riscv_atomic.h>

#ifndef TARGET_PLATFORM_HEADER
#error "SM requires a defined platform to build"
#endif

// Special target platform header, set by configure script
#include TARGET_PLATFORM_HEADER

/* PMP global spin locks */
static spinlock_t pmp_lock = SPIN_LOCK_INITIALIZER;

/* PMP region getter/setters */
static struct pmp_region regions[PMP_MAX_N_REGION];
static uint32_t reg_bitmap = 0;
static uint32_t region_def_bitmap = 0;

static inline int region_register_idx(region_id i)
{
  return regions[i].reg_idx;
}

static inline int region_allows_overlap(region_id i)
{
  return regions[i].allow_overlap;
}

static inline uintptr_t region_get_addr(region_id i)
{
  return regions[i].addr;
}

static inline uint64_t region_get_size(region_id i)
{
  return regions[i].size;
}

static inline int region_is_napot(region_id i)
{
  return regions[i].addrmode == PMP_A_NAPOT;
}

static inline int region_is_tor(region_id i)
{
  return regions[i].addrmode == PMP_A_TOR;
}

static inline int region_needs_two_entries(region_id i)
{
  return region_is_tor(i) && regions[i].reg_idx > 0;
}

static inline int region_is_napot_all(region_id i)
{
  return regions[i].addr == 0 && regions[i].size == -1UL;
}

static inline uintptr_t region_pmpaddr_val(region_id i)
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

static inline uintptr_t region_pmpcfg_val(region_id i, pmpreg_id reg_idx, uint8_t perm_bits)
{
  return (uintptr_t) (regions[i].addrmode | perm_bits) << (8*(reg_idx%PMP_PER_GROUP));
}

static void region_clear_all(region_id i)
{
  regions[i].addr = 0;
  regions[i].size = 0;
  regions[i].addrmode = 0;
  regions[i].allow_overlap = 0;
  regions[i].reg_idx = 0;
  regions[i].is_subregion = false;
  regions[i].has_subregion = false;
}

static void region_init(region_id i,
                        uintptr_t addr,
                        uint64_t size,
                        uint8_t addrmode,
                        int allow_overlap,
                        pmpreg_id reg_idx)
{
  regions[i].addr = addr;
  regions[i].size = size;
  regions[i].addrmode = addrmode;
  regions[i].allow_overlap = allow_overlap;
  regions[i].reg_idx = (addrmode == PMP_A_TOR && reg_idx > 0 ? reg_idx + 1 : reg_idx);
  regions[i].is_subregion = false;
  regions[i].has_subregion = false;
}

static int is_pmp_region_valid(region_id region_idx)
{
  return TEST_BIT(region_def_bitmap, region_idx);
}

static int search_rightmost_unset(uint32_t bitmap, int max, uint32_t mask)
{
  int i = 0;

  sm_assert(max < 32);
  sm_assert(!((mask + 1) & mask));

  while(mask < (1UL << max)) {
    if((~bitmap & mask) == mask)
      return i;
    mask = mask << 1;
    i++;
  }

  return -1;
}

static region_id get_free_region_idx(void)
{
  return search_rightmost_unset(region_def_bitmap, PMP_MAX_N_REGION, 0x1);
}

static pmpreg_id get_free_reg_idx(void)
{
  return search_rightmost_unset(reg_bitmap, PMP_N_REG, 0x1);
}

static pmpreg_id get_conseq_free_reg_idx(void)
{
  return search_rightmost_unset(reg_bitmap, PMP_N_REG, 0x3);
}

/* We do an integery overflow safety check here for the inputs (addr +
 * size).  We do NOT do a safety check on epm_base + epm_size, since
 * only valid region should have been created previously.
 *
 * On a failed addr + size overflow, we return failure, since this
 * cannot be a valid addr and size anyway.
 */
static int detect_region_overlap(uintptr_t addr, uintptr_t size)
{
  void* epm_base;
  size_t epm_size;
  int region_overlap = 0, i;

  // Safety check the addr+size
  uintptr_t input_end;
  if( CHECKED_ADD(addr, size, &input_end)){
    return 1;
  }

  for(i=0; i<PMP_MAX_N_REGION; i++)
  {
    if(!is_pmp_region_valid(i))
      continue;

    if(region_allows_overlap(i)) {
      continue;
    }

    epm_base = (void*) region_get_addr(i);
    epm_size = region_get_size(i);

    // Only looking at valid regions, no need to check epm_base+size
    region_overlap |= ((uintptr_t) epm_base < input_end) &&
                      ((uintptr_t) epm_base + epm_size > addr);
  }

  return region_overlap;
}

int pmp_detect_region_overlap_atomic(uintptr_t addr, uintptr_t size)
{
  int region_overlap = 0;
  spin_lock(&pmp_lock);
  region_overlap = detect_region_overlap(addr, size);
  spin_unlock(&pmp_lock);
  return region_overlap;
}

/*********************************
 *
 * External Functions
 *
 **********************************/

int pmp_unset_global(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_INVALID, "Invalid PMP region index");

  send_and_sync_pmp_ipi(region_idx, SBI_PMP_IPI_TYPE_UNSET, PMP_NO_PERM);

  return SBI_ERR_SM_PMP_SUCCESS;
}

/* populate pmp set command to every other hart */
int pmp_set_global(int region_idx, uint8_t perm)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_INVALID, "Invalid PMP region index");

  send_and_sync_pmp_ipi(region_idx, SBI_PMP_IPI_TYPE_SET, perm);

  return SBI_ERR_SM_PMP_SUCCESS;
}

void pmp_init(void)
{
  uintptr_t pmpaddr = 0;
  uintptr_t pmpcfg = 0;
  int i;
  for (i=0; i < PMP_N_REG; i++)
  {
    switch(i) {
#define X(n,g) case n: { PMP_SET(n, g, pmpaddr, pmpcfg); break; }
      LIST_OF_PMP_REGS
#undef X
    }
  }
}

static int _pmp_set(int region_idx, uint8_t perm_bits, pmpreg_id reg_idx) {
  uintptr_t pmpcfg = region_pmpcfg_val(region_idx, reg_idx, perm_bits);
  uintptr_t pmpaddr = region_pmpaddr_val(region_idx);
  int n=reg_idx;

  switch(n) {
#define X(n,g) case n: { PMP_SET(n, g, pmpaddr, pmpcfg); break; }
  LIST_OF_PMP_REGS
#undef X
    default:
      sm_assert(false);
  }

  /* TOR decoding with 2 registers */
  if(region_needs_two_entries(region_idx))
  {
    n--;
    pmpcfg = 0;
    pmpaddr = region_get_addr(region_idx) >> 2;
    switch(n) {
#define X(n,g) case n: { PMP_SET(n, g, pmpaddr, pmpcfg); break; }
  LIST_OF_PMP_REGS
#undef X
    default:
      sm_assert(false);
    }
  }
  return SBI_ERR_SM_PMP_SUCCESS;
}

int pmp_set_keystone(int region_idx, uint8_t perm)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_INVALID, "Invalid PMP region index");

  uint8_t perm_bits = perm & PMP_ALL_PERM;
  pmpreg_id reg_idx = region_register_idx(region_idx);

  //sbi_printf("pmp_set() [hart %d]: reg[%d], mode[%s], range[0x%lx-0x%lx], perm[0x%x]\r\n",
  //       current_hartid(), reg_idx, (region_is_tor(region_idx) ? "TOR":"NAPOT"),
  //       region_get_addr(region_idx), region_get_addr(region_idx) + region_get_size(region_idx), perm);
  //sbi_printf("  pmp[%d] = pmpaddr: 0x%lx, pmpcfg: 0x%lx\r\n", reg_idx, pmpaddr, pmpcfg);

  return _pmp_set(region_idx, perm_bits, reg_idx);
}

int pmp_unset(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_INVALID,"Invalid PMP region index");

  pmpreg_id reg_idx = region_register_idx(region_idx);
  int n=reg_idx;
  switch(n) {
#define X(n,g) case n: { PMP_UNSET(n, g); break;}
  LIST_OF_PMP_REGS
#undef X
    default:
      sm_assert(false);
  }

  if(region_needs_two_entries(region_idx))
  {
    n--;
    switch(n) {
#define X(n,g) case n: { PMP_UNSET(n,g); break; }
  LIST_OF_PMP_REGS
#undef X
    default:
      sm_assert(false);
    }
  }

  return SBI_ERR_SM_PMP_SUCCESS;
}

int pmp_move(int region_idx, pmpreg_id to)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_INVALID,"Invalid PMP region index");

  // First, have to extract the current configuration
  uintptr_t cfg;
  int n=region_register_idx(region_idx);;

  switch(n) {
#define X(n, g) case n: { cfg = csr_read(pmpcfg##g); break; }
    LIST_OF_PMP_REGS
#undef X
    default:
    sm_assert(FALSE);
  }

  // Then, unset and move the new register
  cfg = (cfg >> ((uintptr_t)8*(n%PMP_PER_GROUP))) & 0xff;
  pmp_unset(region_idx);
  _pmp_set(region_idx, cfg, to);
  return SBI_ERR_SM_PMP_SUCCESS;
}

static int pmp_move_global(int region_idx, pmpreg_id to)
{
  if(!is_pmp_region_valid(region_idx))
      PMP_ERROR(SBI_ERR_SM_PMP_REGION_INVALID, "Invalid PMP region index");

  send_and_sync_pmp_ipi(region_idx, SBI_PMP_IPI_TYPE_MOVE, to);

  return SBI_ERR_SM_PMP_SUCCESS;
}

int pmp_region_subregion_atomic(uintptr_t start, uint64_t size, region_id container, region_id *rid)
{
  int i, ret, reg_tmp;
  region_id res;
  pmpreg_id allocated_reg;

  spin_lock(&pmp_lock);

  // Check that this would actually be a subregion. First,
  // is it actually just identical to the container?
  if(start == regions[container].addr &&
     size == regions[container].size)
  {
    *rid = container;
    ret = SBI_ERR_SM_PMP_SUCCESS;
    goto __done;
  }

  // Not identical -- is the proposed region contained entirely
  // within the container region?
  if((start < regions[container].addr) ||
     (start + size > regions[container].addr + regions[container].size))
  {
    ret = SBI_ERR_SM_PMP_REGION_NOT_SUBREGION;
    goto __done;
  }

  // Cannot allocate subregions of subregions
  if(regions[container].is_subregion)
  {
    ret = SBI_ERR_SM_PMP_REGION_NOT_SUBREGION;
    goto __done;
  }

  ret = pmp_region_init(start, size, PMP_PRI_ANY, &res, 1);
  if(ret == SBI_ERR_SM_PMP_SUCCESS)
  {
    allocated_reg = regions[res].reg_idx;

    regions[res].is_subregion = true;
    regions[container].has_subregion = true;

    // Now, we need to make sure that the container has the lowest
    // priority out of all of its subregions. We maintain the invariant
    // that, whenever we create a subregion, we move the container region
    // into the lowest occupied priority. This means that we would only
    // ever hypothetically need to swap the registers corresponding to the
    // container and the register we just allocated.

    if(allocated_reg > regions[container].reg_idx)
    {
      pmp_move_global(container, allocated_reg);

      reg_tmp = regions[container].reg_idx;
      regions[container].reg_idx = allocated_reg;
      regions[res].reg_idx = reg_tmp;

      if(region_needs_two_entries(res))
      {
        // We need to find the other region which contains the register
        for(i = 0; i < PMP_MAX_N_REGION; i++)
        {
          if(regions[i].reg_idx == regions[res].reg_idx - 1)
          {
            // This is the other register. Can we potentially move this
            // region to the other register value we have just allocated?
            sm_assert(!regions[i].has_subregion && !regions[i].is_subregion)
            sm_assert(!region_needs_two_entries(i));

            // We should be fine, go ahead and swap this as well
            pmp_move_global(i, allocated_reg - 1);
            regions[i].reg_idx = allocated_reg - 1;
            break;
          }
        }
      }

    }

    *rid = res;
  }

__done:
  spin_unlock(&pmp_lock);
  return ret;
}

int pmp_region_init_atomic(uintptr_t start, uint64_t size, enum pmp_priority priority, region_id* rid, int allow_overlap)
{
  int ret;
  spin_lock(&pmp_lock);
  ret = pmp_region_init(start, size, priority, rid, allow_overlap);
  spin_unlock(&pmp_lock);
  return ret;
}

static int tor_region_init(uintptr_t start, uint64_t size, enum pmp_priority priority, region_id* rid, int allow_overlap)
{
  pmpreg_id reg_idx = -1;
  region_id region_idx = -1;

  sm_assert(size);
  sm_assert(!(size & (RISCV_PGSIZE-1)));
  sm_assert(!(start & (RISCV_PGSIZE-1)));
  sm_assert(rid);
  sm_assert(priority != PMP_PRI_BOTTOM);

  region_idx = get_free_region_idx();
  if(region_idx < 0 || region_idx > PMP_MAX_N_REGION)
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_MAX_REACHED, "Reached the maximum number of PMP regions");

  *rid = region_idx;
  switch(priority)
  {
    case(PMP_PRI_ANY): {
      reg_idx = get_conseq_free_reg_idx();
      if(reg_idx < 0)
        PMP_ERROR(SBI_ERR_SM_PMP_REGION_MAX_REACHED, "No available PMP register");
      if(TEST_BIT(reg_bitmap, reg_idx) || TEST_BIT(reg_bitmap, reg_idx + 1) || reg_idx + 1 >= PMP_N_REG)
        PMP_ERROR(SBI_ERR_SM_PMP_REGION_MAX_REACHED, "PMP register unavailable");

      break;
    }
    case(PMP_PRI_TOP): {
      sm_assert(start == 0);
      reg_idx = 0;
      if(TEST_BIT(reg_bitmap, reg_idx))
        PMP_ERROR(SBI_ERR_SM_PMP_REGION_MAX_REACHED, "PMP register unavailable");
      break;
    }
    default: {
      sm_assert(0);
    }
  }

  // initialize the region
  region_init(region_idx, start, size, PMP_A_TOR, allow_overlap, reg_idx);
  SET_BIT(region_def_bitmap, region_idx);
  SET_BIT(reg_bitmap, reg_idx);

  if(reg_idx > 0)
    SET_BIT(reg_bitmap, reg_idx + 1);

  return SBI_ERR_SM_PMP_SUCCESS;
}

static int napot_region_init(uintptr_t start, uint64_t size, enum pmp_priority priority, region_id* rid, int allow_overlap)
{
  pmpreg_id reg_idx = -1;
  region_id region_idx = -1;

  sm_assert(size);
  sm_assert(rid);

  if(!(size == -1UL && start == 0))
  {
    sm_assert(!(size & (size-1)));
    sm_assert(!(start & (size - 1)));
    sm_assert(!(size & (RISCV_PGSIZE-1)));
    sm_assert(!(start & (RISCV_PGSIZE-1)));
  }

  //find avaiable pmp region idx
  region_idx = get_free_region_idx();
  if(region_idx < 0 || region_idx > PMP_MAX_N_REGION)
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_MAX_REACHED, "Reached the maximum number of PMP regions");

  *rid = region_idx;

  switch(priority)
  {
    case(PMP_PRI_ANY): {
      reg_idx = get_free_reg_idx();
      if(reg_idx < 0)
        PMP_ERROR(SBI_ERR_SM_PMP_REGION_MAX_REACHED, "No available PMP register");
      if(TEST_BIT(reg_bitmap, reg_idx) || reg_idx >= PMP_N_REG)
        PMP_ERROR(SBI_ERR_SM_PMP_REGION_MAX_REACHED, "PMP register unavailable");
      break;
    }
    case(PMP_PRI_TOP): {
      reg_idx = 0;
      if(TEST_BIT(reg_bitmap, reg_idx))
        PMP_ERROR(SBI_ERR_SM_PMP_REGION_MAX_REACHED, "PMP register unavailable");
      break;
    }
    case(PMP_PRI_BOTTOM): {
      /* the bottom register can be used by multiple regions,
       * so we don't check its availability */
      reg_idx = PMP_N_REG - 1;
      break;
    }
    default: {
      sm_assert(0);
    }
  }

  // initialize the region
  region_init(region_idx, start, size, PMP_A_NAPOT, allow_overlap, reg_idx);
  SET_BIT(region_def_bitmap, region_idx);
  SET_BIT(reg_bitmap, reg_idx);

  return SBI_ERR_SM_PMP_SUCCESS;
}

static int pmp_region_free(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
  {
    spin_unlock(&pmp_lock);
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_INVALID, "Invalid PMP region index");
  }

  pmpreg_id reg_idx = region_register_idx(region_idx);
  UNSET_BIT(region_def_bitmap, region_idx);
  UNSET_BIT(reg_bitmap, reg_idx);
  if(region_needs_two_entries(region_idx))
    UNSET_BIT(reg_bitmap, reg_idx - 1);

  region_clear_all(region_idx);
    return SBI_ERR_SM_PMP_SUCCESS;
}

int pmp_region_free_atomic(int region_idx)
{
  int ret;
  spin_lock(&pmp_lock);
  ret = pmp_region_free(region_idx);
  spin_unlock(&pmp_lock);
  return ret;
}

int pmp_region_subregion_free_atomic(region_id region) {
    int i, ret = SBI_ERR_SM_PMP_SUCCESS;
    region_id container = -1;

    spin_lock(&pmp_lock);

    // First, find the container of this region
    for(i = 0; i < PMP_MAX_N_REGION; i++) {
        // Skip the OSM region
        if(regions[i].addr == 0 && regions[i].size == -1)
            continue;

        if(regions[region].addr >= regions[i].addr &&
            regions[region].addr + regions[region].size <= regions[i].addr + regions[i].size) {
            // This is the most likely container
            container = i;
            break;
        }
    }

    if(container < 0) {
        ret = SBI_ERR_SM_PMP_REGION_NOT_SUBREGION;
        goto __done;
    }

    if(!regions[container].has_subregion) {
        ret = SBI_ERR_SM_PMP_REGION_NOT_SUBREGION;
        goto __done;
    }

    // Okay, now we can clear all state. We don't need to move any regions back to
    // where they came from originally, since the bitmap will be in a consistent
    // state after this.

    regions[container].has_subregion = false;
    pmp_region_free(region);

__done:
    spin_unlock(&pmp_lock);
    return ret;
}

int pmp_region_init(uintptr_t start, uint64_t size, enum pmp_priority priority, int* rid, int allow_overlap)
{
  if(!size)
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_SIZE_INVALID, "Invalid PMP size");

  /* overlap detection */
  if (!allow_overlap) {
    if (detect_region_overlap(start, size)) {
      return SBI_ERR_SM_PMP_REGION_OVERLAP;
    }
  }

  /* PMP granularity check */
  if(size != -1UL && (size & (RISCV_PGSIZE - 1)))
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_NOT_PAGE_GRANULARITY, "PMP granularity is RISCV_PGSIZE");
  if(start & (RISCV_PGSIZE - 1))
    PMP_ERROR(SBI_ERR_SM_PMP_REGION_NOT_PAGE_GRANULARITY, "PMP granularity is RISCV_PGSIZE");

  /* if the address covers the entire RAM or it's NAPOT */
  if ((size == -1UL && start == 0) ||
      (!(size & (size - 1)) && !(start & (size - 1)))) {
    return napot_region_init(start, size, priority, rid, allow_overlap);
  }
  else
  {
    if(priority != PMP_PRI_ANY &&
      (priority != PMP_PRI_TOP || start != 0)) {
      PMP_ERROR(SBI_ERR_SM_PMP_REGION_IMPOSSIBLE_TOR, "The top-priority TOR PMP entry must start from address 0");
    }

    return tor_region_init(start, size, priority, rid, allow_overlap);
  }
}

uintptr_t pmp_region_get_addr(region_id i)
{
  if(is_pmp_region_valid(i))
    return region_get_addr(i);
  return 0;
}

uint64_t pmp_region_get_size(region_id i)
{
  if(is_pmp_region_valid(i))
    return region_get_size(i);
  return 0;
}

void pmp_dump(void) {
	int i, n;
	uintptr_t cfg, addr;

	sbi_printf("PMP STATE\n");
	for(i = 0; i < PMP_MAX_N_REGION; i++) {
		if(is_pmp_region_valid(i)) {
			sbi_printf("\tregion %i valid\n", i);
			sbi_printf("\t\taddr %lx, size %lx\n", regions[i].addr, regions[i].size);
			sbi_printf("\t\taddrmode %x, overlap %i\n", regions[i].addrmode, regions[i].allow_overlap);
			sbi_printf("\t\tregidx %x\n", regions[i].reg_idx);

			n = regions[i].reg_idx;
			switch(n) {
#define X(n, g) case n: { cfg = csr_read(pmpcfg##g); addr = csr_read(pmpaddr##n); break; }
				LIST_OF_PMP_REGS
#undef X
				default:
				sm_assert(FALSE);
			}

			cfg = (cfg >> ((uintptr_t)8*(n%PMP_PER_GROUP))) & 0xff;
			sbi_printf("\t\t\tcfg %lx ", cfg);

			if(cfg & PMP_L)
				sbi_printf("L");
			if(cfg & PMP_A)
				sbi_printf("A");
			if(cfg & PMP_X)
				sbi_printf("X");
			if(cfg & PMP_W)
				sbi_printf("W");
			if(cfg & PMP_R)
				sbi_printf("R");

			sbi_printf("\n\t\t\taddr %lx\n", addr);
		}
	}
}

//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "pmp.h"
#include "mtrap.h"
#include "atomic.h"
#include "fdt.h"
#include "disabled_hart_mask.h"
#include "safe_math_util.h"

static uint32_t reg_bitmap = 0;
static uint32_t region_def_bitmap = 0;
static struct pmp_region regions[PMP_MAX_N_REGION];

/* PMP IPI mailbox */
typedef struct {
  uint8_t pending;
  uint8_t perm;
} ipi_msg_t;

static ipi_msg_t ipi_mailbox[MAX_HARTS] = {0,};
static int ipi_region_idx = -1;
static enum ipi_type {IPI_PMP_INVALID=-1,
                      IPI_PMP_SET,
                      IPI_PMP_UNSET} ipi_type = IPI_PMP_INVALID;

/* PMP IPI global lock */
static spinlock_t pmp_ipi_global_lock = SPINLOCK_INIT;

static spinlock_t pmp_lock = SPINLOCK_INIT;

static int is_pmp_region_valid(int region_idx) {
  return TEST_BIT(region_def_bitmap, region_idx);
}

int search_rightmost_unset(uint32_t bitmap, int max, uint32_t mask) {
  int i = 0;

  assert(max < 32);
  assert(!((mask + 1) & mask));
  while(mask < (1 << (max-1) ))
  {
    if((~bitmap & mask) == mask)
      return i;
    mask = mask << 1;
    i++;
  }

  return -1;
}

int get_free_region_idx() {
  return search_rightmost_unset(region_def_bitmap, PMP_MAX_N_REGION, 0x1);
}

int get_free_reg_idx() {
  return search_rightmost_unset(reg_bitmap, PMP_N_REG, 0x1);
}

// get consecutive two free reg index for TOR
int get_conseq_free_reg_idx() {
  return search_rightmost_unset(reg_bitmap, PMP_N_REG, 0x11);
}

void handle_pmp_ipi(uintptr_t* regs, uintptr_t dummy, uintptr_t mepc)
{
  if(ipi_type == IPI_PMP_SET)
  {
    uint8_t perm = ipi_mailbox[read_csr(mhartid)].perm;
    pmp_set(ipi_region_idx, perm);
  }
  else
  {
    pmp_unset(ipi_region_idx);
  }

  ipi_mailbox[read_csr(mhartid)].pending = 0;
  return;
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

  /* CAUTION: the caller should use pmp_lock to synchronize */

  // Safety check the addr+size
  uintptr_t input_end;
  if( __checked_uaddl(addr, size, &input_end)){
    return 1;
  }

  for(i=0; i<PMP_MAX_N_REGION; i++)
  {
    if(!is_pmp_region_valid(i))
      continue;

    struct pmp_region region = regions[i];
    if(region.allow_overlap) {
      continue;
    }

    epm_base = (void*) region.addr;
    epm_size = region.size;

    // Only looking at valid regions, no need to check epm_base+size
    region_overlap |= ((uintptr_t) epm_base < input_end) &&
                      ((uintptr_t) epm_base + epm_size > addr);
  }

  return region_overlap;
}

int detect_region_overlap_atomic(uintptr_t addr, uintptr_t size)
{
  int region_overlap = 0;
  spinlock_lock(&pmp_lock);
  region_overlap = detect_region_overlap(addr, size);
  spinlock_unlock(&pmp_lock);
  return region_overlap;
}

static void send_pmp_ipi(uintptr_t recipient, uint8_t perm)
{
  if (((disabled_hart_mask >> recipient) & 1)) return;
  /* never send IPI to my self; it will result in a deadlock */
  if (recipient == read_csr(mhartid)) return;
  atomic_or(&OTHER_HLS(recipient)->mipi_pending, IPI_PMP);
  mb();
  ipi_mailbox[recipient].pending = 1;
  ipi_mailbox[recipient].perm = perm & PMP_ALL_PERM;
  *OTHER_HLS(recipient)->ipi = 1;
}

static void send_and_sync_pmp_ipi(int region_idx, enum ipi_type type, uint8_t perm)
{
  uintptr_t mask = hart_mask;
  ipi_region_idx = region_idx;
  ipi_type = type;

  for(uintptr_t i=0, m=mask; m; i++, m>>=1)
  {
    if(m & 1) {
      send_pmp_ipi(i, perm);
    }
  }

  /* wait until every other hart sets PMP */
  for(uintptr_t i=0, m=mask; m; i++, m>>=1) {
    if(m & 1) {
      while( atomic_read(&ipi_mailbox[i].pending) ) {
        continue;
      }
    }
  }
}

int pmp_unset_global(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(PMP_REGION_INVALID, "Invalid PMP region index");

  /* We avoid any complex PMP-related IPI management
   * by ensuring only one hart can enter this region at a time */
#ifdef __riscv_atomic
  spinlock_lock(&pmp_ipi_global_lock);
  send_and_sync_pmp_ipi(region_idx, IPI_PMP_UNSET, PMP_NO_PERM);
  spinlock_unlock(&pmp_ipi_global_lock);
#endif
  /* unset PMP of itself */
  pmp_unset(region_idx);

  return PMP_SUCCESS;
}

/* populate pmp set command to every other hart */
int pmp_set_global(int region_idx, uint8_t perm)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(PMP_REGION_INVALID, "Invalid PMP region index");

  /* We avoid any complex PMP-related IPI management
   * by ensuring only one hart can enter this region at a time */
#ifdef __riscv_atomic
  spinlock_lock(&pmp_ipi_global_lock);
  send_and_sync_pmp_ipi(region_idx, IPI_PMP_SET, perm);
  spinlock_unlock(&pmp_ipi_global_lock);
#endif
  /* set PMP of itself */
  pmp_set(region_idx, perm);
  return PMP_SUCCESS;
}

int pmp_set(int region_idx, uint8_t perm)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(PMP_REGION_INVALID, "Invalid PMP region index");

  uint8_t perm_bits = perm & PMP_ALL_PERM;
  int reg_idx = regions[region_idx].reg_idx;
  uintptr_t pmpcfg = (uintptr_t) (regions[region_idx].addrmode | perm_bits) << (8*(reg_idx%PMP_PER_GROUP));
  uintptr_t pmpaddr;
  uintptr_t tor_pmpaddr;

  if(regions[region_idx].addrmode == PMP_NAPOT) {
    pmpaddr = regions[region_idx].addr;
  } else {
    tor_pmpaddr = regions[region_idx].start;
    pmpaddr = tor_pmpaddr + regions[region_idx].size;
  }

  /*spinlock_lock(&pmp_lock);
  printm("pmp_set() [hart %d]: pmpreg %d, address:%lx, size:%lx, perm: 0x%x\n",
      read_csr(mhartid), reg_idx, regions[region_idx].addr<<2, regions[region_idx].size, perm);
  spinlock_unlock(&pmp_lock);
  */
  int n=reg_idx;

  switch(n) {
#define X(n,g) case n: { PMP_SET(n, g, pmpaddr, pmpcfg); break; }
  LIST_OF_PMP_REGS
#undef X
    default:
      die("pmp_set failed: this must not be tolerated\n");
  }

  /* TOR decoding with 2 registers */
  if(regions[region_idx].addrmode == PMP_TOR && reg_idx > 0)
  {
    n--;
    pmpcfg = 0;
    switch(n) {
#define X(n,g) case n: { PMP_SET(n, g, tor_pmpaddr, pmpcfg); break; }
  LIST_OF_PMP_REGS
#undef X
    default:
      die("pmp_set failed: this must not be tolerated\n");
    }
  }
  return PMP_SUCCESS;
}

int pmp_unset(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(PMP_REGION_INVALID,"Invalid PMP region index");

  int reg_idx = regions[region_idx].reg_idx;
  int n=reg_idx;
  switch(n) {
#define X(n,g) case n: { PMP_UNSET(n, g); break;}
  LIST_OF_PMP_REGS
#undef X
    default:
      die("pmp_unset failed: this must not be tolerated\n");
  }

  if(regions[region_idx].addrmode == PMP_TOR && reg_idx > 0)
  {
    n--;
    switch(n) {
#define X(n,g) case n: { PMP_UNSET(n,g); break; }
  LIST_OF_PMP_REGS
#undef X
    default:
      die("pmp_unset failed: this must not be tolerated\n");
    }
  }

  /* spinlock_lock(&pmp_lock);
  printm("pmp_unset() [hart %d]: pmpreg %d, address:%lx, size:%lx\n",
      read_csr(mhartid), reg_idx, regions[region_idx].addr<<2, regions[region_idx].size);
  spinlock_unlock(&pmp_lock);
  */
  return PMP_SUCCESS;
}

int pmp_region_debug_print(int region_idx)
{
  if(!is_pmp_region_valid(region_idx))
    PMP_ERROR(PMP_REGION_INVALID,"Invalid PMP region index");

  uintptr_t start = regions[region_idx].start;
  uint64_t size = regions[region_idx].size;
  uint8_t addrmode = regions[region_idx].addrmode;
  uintptr_t addr = regions[region_idx].addr;
  int reg_idx = regions[region_idx].reg_idx;

  printm("start: 0x%llx\n"
    "size: 0x%llx (%d)\n"
    "addrmode: 0x%x\n"
    "addr<<3: 0x%llx\n"
    "reg_idx: %d\n"
   , start, size, size, addrmode, addr<<3, reg_idx );

  return PMP_SUCCESS;
}


int pmp_region_init_atomic(uintptr_t start, uint64_t size, enum pmp_priority priority, int* rid, int allow_overlap)
{
  int ret;
  spinlock_lock(&pmp_lock);
  ret = pmp_region_init(start, size, priority, rid, allow_overlap);
  spinlock_unlock(&pmp_lock);
  return ret;
}

int pmp_tor_region_init(uintptr_t start, uint64_t size, enum pmp_priority priority, int* rid, int allow_overlap)
{
  uintptr_t pmp_address_bottom;
  uintptr_t pmp_address_top;
  int reg_idx = -1;
  int region_idx = -1;
  int region_overlap = 0, i=0;

  assert(size);
  assert(!(size & (RISCV_PGSIZE-1)));
  assert(!(start & (RISCV_PGSIZE-1)));
  assert(rid);
  assert(priority != PMP_PRI_BOTTOM);

  pmp_address_bottom = start;
  pmp_address_top = start + size;

  region_idx = get_free_region_idx();
  if(region_idx < 0 || region_idx > PMP_MAX_N_REGION)
    PMP_ERROR(PMP_REGION_MAX_REACHED, "Reached the maximum number of PMP regions");

  *rid = region_idx;
  switch(priority)
  {
    case(PMP_PRI_ANY): {
      reg_idx = get_conseq_free_reg_idx();
      if(reg_idx < 0)
        PMP_ERROR(PMP_REGION_MAX_REACHED, "No available PMP register");
      if(TEST_BIT(reg_bitmap, reg_idx) || TEST_BIT(reg_bitmap, reg_idx + 1) || reg_idx >= PMP_N_REG)
        PMP_ERROR(PMP_REGION_MAX_REACHED, "PMP register unavailable");

      assert(reg_idx + 1 < PMP_N_REG);
      break;
    }
    case(PMP_PRI_TOP): {
      assert(start == 0);
      reg_idx = 0;
      if(TEST_BIT(reg_bitmap, reg_idx))
        PMP_ERROR(PMP_REGION_MAX_REACHED, "PMP register unavailable");
      break;
    }
    default: {
      assert(0);
    }
  }

  // initialize the region
  regions[region_idx].start = start;
  regions[region_idx].size = size;
  regions[region_idx].addrmode = PMP_TOR;
  regions[region_idx].addr = pmp_address_top;
  // use next reg for config
  regions[region_idx].reg_idx = reg_idx + 1;
  regions[region_idx].allow_overlap = allow_overlap;
  SET_BIT(region_def_bitmap, region_idx);
  SET_BIT(reg_bitmap, reg_idx);
  if(reg_idx > 0)
    SET_BIT(reg_bitmap, reg_idx - 1);

  return PMP_SUCCESS;
}

int pmp_napot_region_init(uintptr_t start, uint64_t size, enum pmp_priority priority, int* rid, int allow_overlap)
{
  uintptr_t pmp_address;
  int reg_idx = -1;
  int region_idx = -1;
  int region_overlap = 0, i = 0;

  assert(size);
  assert(rid);

  /* if region covers the entire RAM */
  if(size == -1UL && start == 0)
  {
    pmp_address = -1UL;
  }
  else
  {
    assert(!(size & (size-1)));
    assert(!(start & (size - 1)));
    assert(!(size & (RISCV_PGSIZE-1)));
    assert(!(start & (RISCV_PGSIZE-1)));

    pmp_address = (start | (size/2-1)) >> 2;
  }

  //find avaiable pmp region idx
  region_idx = get_free_region_idx();
  if(region_idx < 0 || region_idx > PMP_MAX_N_REGION)
    PMP_ERROR(PMP_REGION_MAX_REACHED, "Reached the maximum number of PMP regions");

  *rid = region_idx;

  switch(priority)
  {
    case(PMP_PRI_ANY): {
      reg_idx = get_free_reg_idx();
      if(reg_idx < 0)
        PMP_ERROR(PMP_REGION_MAX_REACHED, "No available PMP register");
      if(TEST_BIT(reg_bitmap, reg_idx) || reg_idx >= PMP_N_REG)
        PMP_ERROR(PMP_REGION_MAX_REACHED, "PMP register unavailable");
      break;
    }
    case(PMP_PRI_TOP): {
      reg_idx = 0;
      if(TEST_BIT(reg_bitmap, reg_idx))
        PMP_ERROR(PMP_REGION_MAX_REACHED, "PMP register unavailable");
      break;
    }
    case(PMP_PRI_BOTTOM): {
      /* the bottom register can be used by multiple regions,
       * so we don't check its availability */
      reg_idx = PMP_N_REG - 1;
      break;
    }
    default: {
      assert(0);
    }
  }

  // initialize the region
  regions[region_idx].start = start;
  regions[region_idx].size = size;
  regions[region_idx].addrmode = PMP_NAPOT;
  regions[region_idx].addr = pmp_address;
  regions[region_idx].reg_idx = reg_idx;
  regions[region_idx].allow_overlap = allow_overlap;
  SET_BIT(region_def_bitmap, region_idx);
  SET_BIT(reg_bitmap, reg_idx);

  return PMP_SUCCESS;
}

int pmp_region_free_atomic(int region_idx)
{

  spinlock_lock(&pmp_lock);

  if(!is_pmp_region_valid(region_idx))
  {
    spinlock_unlock(&pmp_lock);
    PMP_ERROR(PMP_REGION_INVALID, "Invalid PMP region index");
  }
  /*
  if(is_pmp_region_set(region_idx)) {
    int ret = pmp_unset(region_idx);
    if(ret) {
      return ret;
    }
  }*/

  int reg_idx = regions[region_idx].reg_idx;
  UNSET_BIT(region_def_bitmap, region_idx);
  UNSET_BIT(reg_bitmap, reg_idx);
  if(regions[region_idx].addrmode == PMP_TOR && reg_idx > 0)
    UNSET_BIT(reg_bitmap, reg_idx - 1);

  regions[region_idx].start = 0;
  regions[region_idx].size = 0;
  regions[region_idx].addrmode = 0;
  regions[region_idx].addr = 0;
  regions[region_idx].reg_idx = -1;

  spinlock_unlock(&pmp_lock);

  return PMP_SUCCESS;
}

int pmp_region_init(uintptr_t start, uint64_t size, enum pmp_priority priority, int* rid, int allow_overlap)
{
  if(!size)
    PMP_ERROR(PMP_REGION_SIZE_INVALID, "Invalid PMP size");

  /* overlap detection */
  if (!allow_overlap) {
    if (detect_region_overlap(start, size)) {
      return PMP_REGION_OVERLAP;
    }
  }

  /* PMP granularity check */
  if(size != -1UL && (size & (RISCV_PGSIZE - 1)))
    PMP_ERROR(PMP_REGION_NOT_PAGE_GRANULARITY, "PMP granularity is RISCV_PGSIZE");
  if(start & (RISCV_PGSIZE - 1))
    PMP_ERROR(PMP_REGION_NOT_PAGE_GRANULARITY, "PMP granularity is RISCV_PGSIZE");

  /* if the address covers the entire RAM or it's NAPOT */
  if ((size == -1UL && start == 0) ||
      (!(size & (size - 1)) && !(start & (size - 1)))) {
    return pmp_napot_region_init(start, size, priority, rid, allow_overlap);
  }
  else
  {
    /* TOR region is very restricted, since it consumes an adjacent pmp entry */
    if(priority != PMP_PRI_ANY &&
        (priority != PMP_PRI_TOP || start != 0))
    {
      // We could've gave away the two PMP entries, but we just don't.
      PMP_ERROR(PMP_REGION_IMPOSSIBLE_TOR, "The top-priority TOR PMP entry must start from address 0");
    }

    return pmp_tor_region_init(start, size, priority, rid, allow_overlap);
  }
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

uint64_t pmp_get_size(int region_idx)
{
  if(!TEST_BIT(region_def_bitmap, region_idx))
  {
    printm("pmp_get_size(): Invalid PMP region index\n");
    return 0;
  }

  return regions[region_idx].size;
}

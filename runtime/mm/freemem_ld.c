#include "mm/freemem.h"
#include "mm/common.h"
#include "mm/vm_defs.h"
#include "util/string.h"

static uintptr_t freeBase;
static uintptr_t freeEnd;

void spa_init(uintptr_t base, size_t size)
{
  freeBase = base;
  freeEnd = freeBase + size;
}

uintptr_t spa_get()
{
  return spa_get_zero(); // not allowed, so change to safe
}

uintptr_t spa_get_zero()
{
  if (freeBase >= freeEnd) {
    return 0;
  }
  uintptr_t new_page = freeBase;
  memset((void *) new_page, 0, RISCV_PAGE_SIZE);

  freeBase += RISCV_PAGE_SIZE;
  return new_page;
}

void spa_put(uintptr_t page)
{
  assert(false); // not implemented
}

unsigned int spa_available()
{
  return (freeEnd - freeBase) / RISCV_PAGE_SIZE;
}

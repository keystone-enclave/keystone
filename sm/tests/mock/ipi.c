#include <sbi/sbi_tlb.h>

void __wrap_send_and_sync_pmp_ipi(int region_idx, int type, uint8_t perm) 
{
  return;
}

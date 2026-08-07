#include <sbi/sbi_fifo.h>
#include <sbi/sbi_ipi.h>
#include <sbi/sbi_scratch.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_console.h>
#include <sbi/sbi_hsm.h>
#include <sbi/sbi_domain.h>
#include "ipi.h"
#include "pmp.h"

void sbi_pmp_ipi_local_update(struct sbi_tlb_info *__info)
{
  struct sbi_pmp_ipi_info* info = (struct sbi_pmp_ipi_info *) __info;
  if (info->type == SBI_PMP_IPI_TYPE_SET) {
    pmp_set_keystone(info->rid, (uint8_t) info->perm);
  } else {
    pmp_unset(info->rid);
  }
}

void send_and_sync_pmp_ipi(int region_idx, int type, uint8_t perm)
{
  ulong mask = 0;
  ulong source_hart = current_hartid();
  struct sbi_tlb_info tlb_info;
  sbi_hsm_hart_interruptible_mask(sbi_domain_thishart_ptr(), 0, &mask);

  SBI_TLB_INFO_INIT(&tlb_info, type, 0, region_idx, perm,
      sbi_pmp_ipi_local_update, source_hart);
  sbi_tlb_request(mask, 0, &tlb_info);
}

void sbi_flush_tlb_local(struct sbi_tlb_info *__info)
{
  asm volatile("sfence.vma");
}

void send_flush_tlb_ipi(void)
{
  ulong mask = 0;
  ulong source_hart = current_hartid();
  struct sbi_tlb_info tlb_info;
  sbi_hsm_hart_interruptible_mask(sbi_domain_thishart_ptr(), 0, &mask);

  SBI_TLB_INFO_INIT(&tlb_info, 0, 0, 0, 0,
      sbi_flush_tlb_local, source_hart);
  sbi_tlb_request(mask, 0, &tlb_info);
}


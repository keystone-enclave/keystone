#include <sbi/sbi_fifo.h>
#include <sbi/sbi_ipi.h>
#include <sbi/sbi_scratch.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_console.h>
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


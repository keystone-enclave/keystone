#ifndef __PMP_IPI_H__
#define __PMP_IPI_H__

#include "pmp.h"
#include <sbi/sbi_scratch.h>
#include <sbi/sbi_hartmask.h>
#define SBI_PMP_IPI_TYPE_SET    0
#define SBI_PMP_IPI_TYPE_UNSET  1

#define SBI_PMP_IPI_FIFO_NUM_ENTRIES  8

struct sbi_pmp_ipi_info {
  int type;
  region_id rid;
  uint8_t perm;
  struct sbi_hartmask smask;
};

#define SBI_PMP_IPI_INFO_SIZE sizeof(struct sbi_pmp_ipi_info)

int sbi_pmp_ipi_init(struct sbi_scratch* scratch, bool cold_boot);

int sbi_pmp_ipi_request(ulong hmask, ulong hbase, struct sbi_pmp_ipi_info* info);
#endif

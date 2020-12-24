#include <sbi/sbi_fifo.h>
#include <sbi/sbi_ipi.h>
#include <sbi/sbi_scratch.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_console.h>
#include "ipi.h"

static unsigned long pmp_ipi_sync_off;
static unsigned long pmp_ipi_fifo_off;
static unsigned long pmp_ipi_fifo_mem_off;

static void sbi_pmp_ipi_local_update(struct sbi_pmp_ipi_info *info)
{
  if (info->type == SBI_PMP_IPI_TYPE_SET) {
    pmp_set_keystone(info->rid, info->perm);
  } else {
    pmp_unset(info->rid);
  }
}

static void sbi_pmp_ipi_entry_process(struct sbi_pmp_ipi_info *info)
{
  u32 rhartid;
  struct sbi_scratch *rscratch = NULL;
  unsigned long *rpmp_sync = NULL;

  sbi_pmp_ipi_local_update(info);

  sbi_hartmask_for_each_hart(rhartid, &info->smask) {
    rscratch = sbi_hartid_to_scratch(rhartid);
    if (!rscratch)
      continue;
    rpmp_sync = sbi_scratch_offset_ptr(rscratch, pmp_ipi_sync_off);
    while (atomic_raw_xchg_ulong(rpmp_sync, 1));
  }
}

static void sbi_pmp_ipi_process_count(struct sbi_scratch *scratch, int count)
{
  struct sbi_pmp_ipi_info info;
  u32 deq_count = 0;
  struct sbi_fifo *pmp_ipi_fifo =
    sbi_scratch_offset_ptr(scratch, pmp_ipi_fifo_off);

  while (!sbi_fifo_dequeue(pmp_ipi_fifo, &info)) {
    sbi_pmp_ipi_entry_process(&info);
    deq_count ++;
    if (deq_count > count)
      break;
  }
}

static void sbi_pmp_ipi_sync(struct sbi_scratch *scratch)
{
  unsigned long *pmp_sync =
    sbi_scratch_offset_ptr(scratch, pmp_ipi_sync_off);

  while (!atomic_raw_xchg_ulong(pmp_sync, 0)) {
    sbi_pmp_ipi_process_count(scratch, 1);
  }
}

static void sbi_pmp_ipi_process(struct sbi_scratch* scratch)
{
  struct sbi_pmp_ipi_info info;
  struct sbi_fifo *pmp_ipi_fifo =
    sbi_scratch_offset_ptr(scratch, pmp_ipi_fifo_off);

  while (!sbi_fifo_dequeue(pmp_ipi_fifo, &info))
    sbi_pmp_ipi_entry_process(&info);
}

static int sbi_pmp_ipi_update(struct sbi_scratch *scratch,
    struct sbi_scratch *remote_scratch,
    u32 remote_hartid, void *data)
{
  struct sbi_fifo *pmp_ipi_fifo_r;
  struct sbi_pmp_ipi_info *info = data;
  u32 curr_hartid = current_hartid();

  if (remote_hartid == curr_hartid) {
    sbi_pmp_ipi_local_update(info);
    return -1;
  }

  pmp_ipi_fifo_r = sbi_scratch_offset_ptr(remote_scratch, pmp_ipi_fifo_off);

  while (sbi_fifo_enqueue(pmp_ipi_fifo_r, data) < 0) {
    sbi_pmp_ipi_process_count(scratch, 1);
  }

  return 0;
}

static struct sbi_ipi_event_ops pmp_ipi_ops = {
  .name = "PMP_IPI",
  .process = sbi_pmp_ipi_process,
  .sync = sbi_pmp_ipi_sync,
  .update = sbi_pmp_ipi_update,
};

static u32 pmp_ipi_event = SBI_IPI_EVENT_MAX;

int sbi_pmp_ipi_request(ulong hmask, ulong hbase, struct sbi_pmp_ipi_info *info)
{
  return sbi_ipi_send_many(hmask, hbase, pmp_ipi_event, info);
}

int sbi_pmp_ipi_init(struct sbi_scratch* scratch, bool cold_boot)
{
  int ret;
  void* pmp_ipi_mem;
  struct sbi_fifo *pmp_ipi_q;
  unsigned long* pmp_sync;

  if (cold_boot) {
    pmp_ipi_sync_off = sbi_scratch_alloc_offset(sizeof(*pmp_sync), "PMP_IPI_SYNC");

    if (!pmp_ipi_sync_off)
      return SBI_ENOMEM;

    pmp_ipi_fifo_off = sbi_scratch_alloc_offset(sizeof(*pmp_ipi_q), "PMP_IPI_FIFO");

    if (!pmp_ipi_fifo_off) {
      sbi_scratch_free_offset(pmp_ipi_sync_off);
      return SBI_ENOMEM;
    }
    pmp_ipi_fifo_mem_off = sbi_scratch_alloc_offset(
        SBI_PMP_IPI_FIFO_NUM_ENTRIES * SBI_PMP_IPI_INFO_SIZE,
        "PMP_IPI_FIFO_MEM");
    if (!pmp_ipi_fifo_mem_off) {
      sbi_scratch_free_offset(pmp_ipi_sync_off);
      sbi_scratch_free_offset(pmp_ipi_fifo_off);
      return SBI_ENOMEM;
    }
    ret = sbi_ipi_event_create(&pmp_ipi_ops);
    if (ret < 0) {
      sbi_scratch_free_offset(pmp_ipi_sync_off);
      sbi_scratch_free_offset(pmp_ipi_fifo_off);
      sbi_scratch_free_offset(pmp_ipi_fifo_mem_off);
      return ret;
    }
    pmp_ipi_event = ret;
  } else {
    if (!pmp_ipi_sync_off ||
        !pmp_ipi_fifo_off ||
        !pmp_ipi_fifo_mem_off)
      return SBI_ENOMEM;
    if (SBI_IPI_EVENT_MAX <= pmp_ipi_event)
      return SBI_ENOSPC;
  }

  pmp_sync = sbi_scratch_offset_ptr(scratch, pmp_ipi_sync_off);
  pmp_ipi_q = sbi_scratch_offset_ptr(scratch, pmp_ipi_fifo_off);
  pmp_ipi_mem = sbi_scratch_offset_ptr(scratch, pmp_ipi_fifo_mem_off);

  *pmp_sync = 0;

  sbi_fifo_init(pmp_ipi_q, pmp_ipi_mem, SBI_PMP_IPI_FIFO_NUM_ENTRIES, SBI_PMP_IPI_INFO_SIZE);

  return 0;
}

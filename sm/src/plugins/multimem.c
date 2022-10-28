#include "plugins/multimem.h"
#include "sm.h"
#include <sbi/sbi_console.h>
#include "mprv.h"

uintptr_t multimem_get_other_region_size(enclave_id eid, size_t *size_out)
{
  int mem_id = get_enclave_region_index(eid, REGION_OTHER);
  if (mem_id == -1)
    return -1;
  size_t out = get_enclave_region_size(eid, mem_id);
  return copy_word_from_sm((uintptr_t)size_out, &out);
}

uintptr_t multimem_get_other_region_addr(enclave_id eid, size_t *size_out)
{
  int mem_id = get_enclave_region_index(eid, REGION_OTHER);
  if (mem_id == -1)
    return -1;
  size_t out = get_enclave_region_base(eid, mem_id);
  return copy_word_from_sm((uintptr_t)size_out, &out);
}

uintptr_t do_sbi_multimem(enclave_id eid, uintptr_t call_id, uintptr_t arg0)
{
  switch(call_id)
  {
    case MULTIMEM_GET_OTHER_REGION_SIZE:
      return multimem_get_other_region_size(eid, (size_t *)arg0);
    case MULTIMEM_GET_OTHER_REGION_ADDR:
      return multimem_get_other_region_addr(eid, (size_t *)arg0);
    default:
      return 0;
  }
  return 0;
}

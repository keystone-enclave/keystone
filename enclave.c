#include "enclave.h"
#include "pmp.h"
#include <string.h>

#define ENCL_MAX  16
static uint64_t encl_bitmap = 0;

#define RET_ON_ERR(ret) {if(ret<0) return ret;}

struct enclave_t enclaves[ENCL_MAX];

int encl_alloc_idx()
{
  int i;
  for(i=0; i<ENCL_MAX; i++)
  {
    if(!(encl_bitmap & (0x1 << i)))
      break;
  }

  if(i == ENCL_MAX)
    return -1;

  SET_BIT(encl_bitmap, i); 
  return i;
}

int encl_free_idx(int idx)
{
  if(!TEST_BIT(encl_bitmap, idx))
    return -1;

  UNSET_BIT(encl_bitmap, idx);
  return 0;
}

int create_enclave(uintptr_t base, uintptr_t size)
{
  uint8_t perm = 0;
  int ret, region;
  
  // TODO: check if base/size legitimate
  // 1. reserve pmp range
  ret = pmp_region_init(base, size, perm);
  RET_ON_ERR(ret);

  region = ret;

  // 2. set pmp
  ret = pmp_set(region); 
  RET_ON_ERR(ret);

  // 3. allocate eid
  ret = encl_alloc_idx();
  RET_ON_ERR(ret);

  enclaves[ret].eid = ret;
  enclaves[ret].rid = region;
  enclaves[ret].state = FRESH;

  return 0;
}

int destroy_enclave(int eid)
{
  if(!TEST_BIT(encl_bitmap, eid))
    return -1;

  // 1. clear all the data in the enclave page
  // TODO
  
  // 2. free pmp range pages
  // TODO Page table not implemented 
  
  // 3. unset pmp
  pmp_region_free(enclaves[eid].rid);
  // 4. release eid
  encl_free_idx(eid);
  
  return 0;
}

int copy_to_enclave(int eid, void* ptr, size_t size)
{
  struct enclave_t encl = enclaves[eid];
  void* epm = pmp_get_addr(encl.rid);
  
  memcpy(epm, ptr, size);
  return 0;
}

int copy_from_enclave(int eid, void* ptr, size_t size)
{
  struct enclave_t encl = enclaves[eid];
  void* epm = pmp_get_addr(encl.rid);

  memcpy(ptr, epm, size);
  return 0;
}

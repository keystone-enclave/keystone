//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include <linux/dma-mapping.h>
#include "keystone.h"
/* idr for enclave UID to struct enclave */
DEFINE_IDR(idr_enclave);
DEFINE_MUTEX(idr_enclave_lock);

#define ENCLAVE_IDR_MIN 0x1000
#define ENCLAVE_IDR_MAX 0xffff

unsigned long calculate_required_pages(
    unsigned long eapp_sz,
    unsigned long eapp_stack_sz,
    unsigned long rt_sz,
    unsigned long rt_stack_sz)
{
  unsigned long req_pages = 0;

  req_pages += PAGE_UP(eapp_sz)/PAGE_SIZE;
  req_pages += PAGE_UP(eapp_stack_sz)/PAGE_SIZE;
  req_pages += PAGE_UP(rt_sz)/PAGE_SIZE;
  req_pages += PAGE_UP(rt_stack_sz)/PAGE_SIZE;

  // FIXME: calculate the required number of pages for the page table.
  // For now, we must allocate at least 1 (top) + 2 (enclave) + 2 (runtime) pages for pg tables
  req_pages += 15;
  return req_pages;
}

/* Smart destroy, handles partial initialization of epm and utm etc */
int destroy_enclave(struct enclave* enclave)
{
  struct epm* epm;
  struct utm* utm;
  if (enclave == NULL)
    return -ENOSYS;

  epm = enclave->epm;
  utm = enclave->utm;

  if (epm)
  {
    epm_destroy(epm);
    kfree(epm);
  }
  if (utm)
  {
    utm_destroy(utm);
    kfree(utm);
  }
  kfree(enclave);
  return 0;
}

struct enclave* create_enclave(unsigned long min_pages)
{
  struct enclave* enclave;

  enclave = kmalloc(sizeof(struct enclave), GFP_KERNEL);
  if (!enclave){
    keystone_err("failed to allocate enclave struct\n");
    goto error_no_free;
  }

  enclave->eid = -1;
  enclave->utm = NULL;
  enclave->close_on_pexit = 1;

  enclave->epm = kmalloc(sizeof(struct epm), GFP_KERNEL);
  enclave->is_init = true;
  if (!enclave->epm)
  {
    keystone_err("failed to allocate epm\n");
    goto error_destroy_enclave;
  }

  if(epm_init(enclave->epm, min_pages)) {
    keystone_err("failed to initialize epm\n");
    goto error_destroy_enclave;
  }
  return enclave;

 error_destroy_enclave:
  destroy_enclave(enclave);
 error_no_free:
  return NULL;
}

unsigned int enclave_idr_alloc(struct enclave* enclave)
{
  unsigned int ueid;

  mutex_lock(&idr_enclave_lock);
  ueid = idr_alloc(&idr_enclave, enclave, ENCLAVE_IDR_MIN, ENCLAVE_IDR_MAX, GFP_KERNEL);
  mutex_unlock(&idr_enclave_lock);

  if (ueid < ENCLAVE_IDR_MIN || ueid >= ENCLAVE_IDR_MAX) {
    keystone_err("failed to allocate UID\n");
    return 0;
  }

  return ueid;
}

struct enclave* enclave_idr_remove(unsigned int ueid)
{
  struct enclave* enclave;
  mutex_lock(&idr_enclave_lock);
  enclave = idr_remove(&idr_enclave, ueid);
  mutex_unlock(&idr_enclave_lock);
  return enclave;
}

struct enclave* get_enclave_by_id(unsigned int ueid)
{
  struct enclave* enclave;
  mutex_lock(&idr_enclave_lock);
  enclave = idr_find(&idr_enclave, ueid);
  mutex_unlock(&idr_enclave_lock);
  return enclave;
}

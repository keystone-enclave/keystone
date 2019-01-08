//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "keystone.h" 
/* idr for enclave UID to enclave_t */
DEFINE_IDR(idr_enclave);
DEFINE_SPINLOCK(idr_enclave_lock);

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
int destroy_enclave(enclave_t* enclave)
{
  epm_t* epm;
  utm_t* utm;
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

enclave_t* create_enclave(unsigned long min_pages)
{
  vaddr_t epm_vaddr;
  unsigned long order = ilog2(min_pages - 1) + 1;
  unsigned long count = 0x1 << order;
  epm_t* epm;
  enclave_t* enclave;

  enclave = kmalloc(sizeof(enclave_t), GFP_KERNEL);
  if (!enclave){
    keystone_err("keystone_create_epm(): failed to allocate enclave struct\n");
    goto error_no_free;
  }

  enclave->utm = NULL;
  enclave->epm = NULL;

  /* allocate contiguous memory */

  epm_vaddr = __get_free_pages(GFP_HIGHUSER, order);
  if(!epm_vaddr) {
    keystone_err("keystone_create_epm(): failed to allocate %lu page(s)\n", count);
    goto error_destroy_enclave;
  }

  /* initialize */
  memset((void*)epm_vaddr, 0, PAGE_SIZE*count);

  epm = kmalloc(sizeof(epm_t), GFP_KERNEL);
  if (!epm)
  {
    keystone_err("keystone_create_epm(): failed to allocate epm\n");
    goto error_destroy_enclave;
  }
 
  INIT_LIST_HEAD(&epm->epm_free_list);
  epm->pa = __pa(epm_vaddr);
  epm->order = order;
  epm_init(epm, epm_vaddr, count);
  enclave->epm = epm;
  return enclave;

 error_destroy_enclave:
  destroy_enclave(enclave);
 error_no_free:
  return NULL;
}

unsigned int enclave_idr_alloc(enclave_t* enclave)
{
  unsigned int ueid;

  spin_lock_bh(&idr_enclave_lock);
  ueid = idr_alloc(&idr_enclave, enclave, ENCLAVE_IDR_MIN, ENCLAVE_IDR_MAX, GFP_KERNEL);
  spin_unlock_bh(&idr_enclave_lock);
  
  if (ueid < ENCLAVE_IDR_MIN || ueid >= ENCLAVE_IDR_MAX) {
    keystone_err("failed to allocate UID\n");
    return 0;
  }
  
  return ueid;
}

enclave_t* enclave_idr_remove(unsigned int ueid) 
{
  enclave_t* enclave;
  spin_lock_bh(&idr_enclave_lock);
  enclave = idr_remove(&idr_enclave, ueid);
  spin_unlock_bh(&idr_enclave_lock);
  return enclave;
}

enclave_t* get_enclave_by_id(unsigned int ueid)
{
  enclave_t* enclave;
  spin_lock_bh(&idr_enclave_lock);
  enclave = idr_find(&idr_enclave, ueid); 
  spin_unlock_bh(&idr_enclave_lock);
  return enclave;
}

//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
//#include <asm/io.h>
//#include <asm/page.h>
#include "keystone.h"
#include "keystone-sbi.h"

#include <linux/dma-mapping.h>
#include <linux/mm.h>
#include <linux/file.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/miscdevice.h>
#include "keystone_user.h"
#define   DRV_DESCRIPTION   "keystone enclave"
#define   DRV_VERSION       "1.0.0"

MODULE_DESCRIPTION(DRV_DESCRIPTION);
MODULE_AUTHOR("Dayeol Lee <dayeol@berkeley.edu>");
MODULE_VERSION(DRV_VERSION);
MODULE_LICENSE("Dual BSD/GPL");

static const struct file_operations keystone_fops = {
    .owner          = THIS_MODULE,
    .mmap           = keystone_mmap,
    .unlocked_ioctl = keystone_ioctl,
    .release        = keystone_release
};

struct miscdevice keystone_dev = {
  .minor = MISC_DYNAMIC_MINOR,
  .name = "keystone_enclave",
  .fops = &keystone_fops,
  .mode = 0666,
};

int keystone_mmap(struct file* filp, struct vm_area_struct *vma)
{
  struct utm* utm;
  struct epm* epm;
  struct enclave* enclave;
  unsigned long vsize, psize;
  vaddr_t paddr;
  enclave = get_enclave_by_id((unsigned long) filp->private_data);
  if(!enclave) {
    keystone_err("invalid enclave id\n");
    return -EINVAL;
  }

  utm = enclave->utm;
  epm = enclave->epm;
  vsize = vma->vm_end - vma->vm_start;

  if(enclave->is_init){
    if (vsize > PAGE_SIZE)
      return -EINVAL;
    paddr = __pa(epm->root_page_table) + (vma->vm_pgoff << PAGE_SHIFT);
    remap_pfn_range(vma,
                    vma->vm_start,
                    paddr >> PAGE_SHIFT,
                    vsize, vma->vm_page_prot);
  }
  else
  {
    psize = utm->size;
    if (vsize > psize)
      return -EINVAL;
    remap_pfn_range(vma,
                    vma->vm_start,
                    __pa(utm->ptr) >> PAGE_SHIFT,
                    vsize, vma->vm_page_prot);
  }
  return 0;
}

static int __init keystone_dev_init(void)
{
  int  ret;


  ret = misc_register(&keystone_dev);
  if (ret < 0)
  {
    pr_err("keystone_enclave: misc_register() failed\n");
  }

  keystone_dev.this_device->coherent_dma_mask = DMA_BIT_MASK(32);

  pr_info("keystone_enclave: " DRV_DESCRIPTION " v" DRV_VERSION "\n");
  return ret;
}

static void __exit keystone_dev_exit(void)
{
  pr_info("keystone_enclave: keystone_dev_exit()\n");
  misc_deregister(&keystone_dev);
  return;
}

module_init(keystone_dev_init);
module_exit(keystone_dev_exit);

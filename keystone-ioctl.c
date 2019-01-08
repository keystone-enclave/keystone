//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "keystone.h"
#include "keystone-sbi-arg.h"
#include "keystone_user.h"
#include <linux/uaccess.h>


int keystone_create_enclave(struct file* filp, unsigned long arg)
{
  int ret;
  
  /* create parameters */
  struct keystone_ioctl_create_enclave *enclp = (struct keystone_ioctl_create_enclave*) arg;
  
  // User ELF
  void* __user user_elf_ptr = (void*) enclp->user_elf_ptr;
  size_t user_elf_size = enclp->user_elf_size;
  size_t  user_stack_size = enclp->user_stack_size;

  // Runtime ELF
  void* __user runtime_elf_ptr = (void*) enclp->runtime_elf_ptr;
  size_t  runtime_elf_size = enclp->runtime_elf_size;
  size_t  runtime_stack_size = enclp->runtime_stack_size;

  // Untrusted mmap size
  void* untrusted_ptr = (void*) enclp->params.untrusted_ptr;
  size_t untrusted_size = enclp->params.untrusted_size;
 
  // runtime parameters
  struct keystone_sbi_create_t create_args;

  enclave_t* enclave;

  /* local variables */
  unsigned long rt_offset;
  unsigned long min_pages = calculate_required_pages(user_elf_size, user_stack_size, runtime_elf_size, runtime_stack_size);
  struct utm_t* utm;
  
  /* argument validity */
  if (!user_elf_ptr || !runtime_elf_ptr || !user_elf_size || !runtime_elf_size){
    ret = -EINVAL;
    goto error_no_free;
  }


  enclave = create_enclave(min_pages);
  if(enclave == NULL){
    ret = -ENOMEM;
    goto error_no_free;
  }
  
  /* initialize runtime */
  ret = -EFAULT;
  if (keystone_rtld_init_runtime(enclave, runtime_elf_ptr, runtime_elf_size, runtime_stack_size, &rt_offset)) {
    keystone_err("failed to initialize runtime\n");
    goto error_destroy_enclave;
  }

  /* initialize user app */
  if (keystone_rtld_init_app(enclave, user_elf_ptr, user_elf_size, user_stack_size, rt_offset)) {
    keystone_err("failed to initialize app\n");
    goto error_destroy_enclave;
  }

  /* Untrusted Memory */
  utm = kmalloc(sizeof(struct utm_t), GFP_KERNEL);
  if (!utm) {
    ret = -ENOMEM;
    goto error_destroy_enclave;
  }

  ret = utm_init(utm, untrusted_size);
  if (ret != 0)
    goto error_destroy_enclave;

  filp->private_data = utm;
  enclave->utm = utm; 

  if (keystone_rtld_init_untrusted(enclave, untrusted_ptr, untrusted_size)) {
    keystone_err("failed to initialize untrusted memory\n");
    goto error_destroy_enclave;
  }

  /* SBI Call */
  create_args.epm_region.paddr = enclave->epm->pa;
  create_args.epm_region.size = enclave->epm->total;
  create_args.utm_region.paddr = __pa(utm->ptr);
  create_args.utm_region.size = utm->size;

  create_args.params = enclp->params;

  // SM will write the eid to enclave_t.eid
  create_args.eid_pptr =  (unsigned int*)__pa(&enclave->eid);
  ret = SBI_CALL_1(SBI_SM_CREATE_ENCLAVE, __pa(&create_args));
  if (ret)
  {
    keystone_err("keystone_create_enclave: SBI call failed\n");
    goto error_destroy_enclave;
  }

  /* allocate UID */
  enclp->eid = enclave_idr_alloc(enclave);

  /* We cleanup the free lists here since the kernel will no longer be
     managing them, they are part of the enclave now. */
  utm_clean_free_list(utm);
  epm_clean_free_list(enclave->epm);
  
  return 0;

 error_destroy_enclave:
  /* This can handle partial initialization failure */
  destroy_enclave(enclave);
  
 error_no_free:
  return ret;
}

int keystone_destroy_enclave(struct file* filp, unsigned long arg)
{
  int ret;
  struct keystone_ioctl_create_enclave *enclp = (struct keystone_ioctl_create_enclave*) arg;
  unsigned long ueid = enclp->eid;
  enclave_t* enclave;
  enclave = get_enclave_by_id(ueid);

  ret = SBI_CALL_1(SBI_SM_DESTROY_ENCLAVE, enclave->eid);
  if(ret) {
    keystone_err("fatal: cannot destroy enclave: SBI failed\n");
    return ret;
  }

  destroy_enclave(enclave);
  enclave_idr_remove(ueid);

  return 0;
}

int keystone_run_enclave(struct file* filp, unsigned long arg)
{
  int ret = 0;
  struct keystone_ioctl_run_enclave *run = (struct keystone_ioctl_run_enclave*) arg;
  unsigned long ueid = run->eid;
  enclave_t* enclave;
  enclave = get_enclave_by_id(ueid);
  
  ret = SBI_CALL_1(SBI_SM_RUN_ENCLAVE, enclave->eid);
  /* if the enclave is timer-interrupted, just resume the enclave */
  while(ret == ENCLAVE_INTERRUPTED)
  {
    ret = SBI_CALL_1(SBI_SM_RESUME_ENCLAVE, enclave->eid);
  }

  return ret;
}

int keystone_resume_enclave(struct file* filp, unsigned long arg)
{
  int ret = 0;
  struct keystone_ioctl_run_enclave *resume = (struct keystone_ioctl_run_enclave*) arg;
  unsigned long ueid = resume->eid;
  enclave_t* enclave;
  enclave = get_enclave_by_id(ueid);
  
  ret = SBI_CALL_1(SBI_SM_RESUME_ENCLAVE, enclave->eid);
  while(ret == ENCLAVE_INTERRUPTED)
  {
    ret = SBI_CALL_1(SBI_SM_RESUME_ENCLAVE, enclave->eid);
  }

  return ret;
}

long keystone_ioctl(struct file *filep, unsigned int cmd, unsigned long arg)
{
  long ret;
  char data[256];

  //pr_info("keystone_enclave: keystone_ioctl() command = %d\n",cmd);

  if(!arg)
    return -EINVAL;
  
  if(copy_from_user(data, (void __user*) arg, _IOC_SIZE(cmd)))
    return -EFAULT;

  switch(cmd)
  {
    case KEYSTONE_IOC_CREATE_ENCLAVE:
      ret = keystone_create_enclave(filep, (unsigned long) data);
      break;
    case KEYSTONE_IOC_DESTROY_ENCLAVE:
      ret = keystone_destroy_enclave(filep, (unsigned long) data);
      break;
    case KEYSTONE_IOC_RUN_ENCLAVE:
      ret = keystone_run_enclave(filep, (unsigned long) data);
      break;
    case KEYSTONE_IOC_RESUME_ENCLAVE:
      ret = keystone_resume_enclave(filep, (unsigned long) data);
      break;
    default:
      return -ENOSYS;
  }

  if(copy_to_user((void __user*) arg, data, _IOC_SIZE(cmd)))
    return -EFAULT;
  
  return ret;
}



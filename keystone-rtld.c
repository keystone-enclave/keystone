//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
// Filename: keystone-rtld.c
// Description: Keystone enclave runtime loader
// Author: Dayeol Lee <dayeol@berkeley.edu> 

#include <linux/elf.h>
#include <linux/binfmts.h>
#include <linux/slab.h>
#include <linux/file.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include "keystone.h"


void debug_dump(char* ptr, unsigned long size)
{
  int i;
  char buf[16];
  int allzeroline = 0;
  int lineiszero = 1;

  pr_info("debug memory dump from virtual address %p (%lu bytes)\n", ptr, size);
  
  for (i=0; i<size; i++) {
    buf[i%16] = ptr[i];
    if(ptr[i] != '\0') {
      lineiszero = 0;
    }

    if(i % 16 == 15) {
      if(lineiszero) {
        allzeroline++;
      }
      else {
        if(allzeroline > 0)
          pr_info("*\n");
        allzeroline = 0;
        pr_info("%08x: %04x %04x %04x %04x %04x %04x %04x %04x\n",
          i-0xf, 
          *((uint16_t*)&buf[0]), 
          *((uint16_t*)&buf[2]), 
          *((uint16_t*)&buf[4]), 
          *((uint16_t*)&buf[6]), 
          *((uint16_t*)&buf[8]), 
          *((uint16_t*)&buf[10]), 
          *((uint16_t*)&buf[12]), 
          *((uint16_t*)&buf[14]));
      }
      lineiszero = 1;
    }
  }
}

void rtld_setup_stack(epm_t* epm, vaddr_t stack_addr, unsigned long size)
{
  vaddr_t va_start = PAGE_DOWN(stack_addr - (size - 1));
  //vaddr_t va_end = PAGE_UP(stack_addr - 1);
  vaddr_t va;
  int i;

  //pr_info("[rt stack] va_start: 0x%lx, va_end: 0x%lx\n", va_start, va_end);

  for(i=0, va=va_start; i< (size>>12); i++, va+=PAGE_SIZE) 
  {
    //pr_info("mapping: %lx\n",va);
    vaddr_t epm_page;
    epm_page = epm_alloc_rt_page_noexec(epm, va);
  }
}


void rtld_vm_mmap(epm_t* epm, vaddr_t encl_addr, unsigned long size,
   void* __user rt_ptr, struct elf_phdr* phdr)
{
  unsigned int k;
  vaddr_t va_start = PAGE_DOWN(encl_addr);
  vaddr_t va_end = PAGE_UP(encl_addr + size);
  vaddr_t va;

  //pr_info("va_start: 0x%lx, va_end: 0x%lx\n", va_start, va_end);

  unsigned long pos = phdr->p_offset;
  for(va=va_start, k=0; va < va_end; va += PAGE_SIZE, k++)
  {
    vaddr_t epm_page;
    epm_page = epm_alloc_rt_page(epm, va);
    //pr_info("encl_mmap va: 0x%lx, target: 0x%lx\n", va, epm_page);
    if(copy_from_user((void*)epm_page, rt_ptr + pos, PAGE_SIZE)){
      keystone_err("failed to copy runtime\n");
    }
    pos += PAGE_SIZE;
   
    //debug_dump(epm_page, PAGE_SIZE);
  }
} 

int keystone_app_load_elf_region(epm_t* epm, void* __user elf_usr_region,
				 void* target_vaddr, size_t len){ 
  vaddr_t va;
  vaddr_t encl_page;
  int k, ret = 0;
  size_t copy_size;
  for(va=(uintptr_t)target_vaddr, k=0; va < (uintptr_t)target_vaddr+len; va += PAGE_SIZE, k++){

    encl_page = epm_alloc_user_page(epm, va);

    copy_size = (k+1)*PAGE_SIZE > len ? len%PAGE_SIZE : PAGE_SIZE;
    //pr_info("Copy elf page to:%x, from: %x, sz:%i\n",
    //	    encl_page, elf_usr_region + (k * PAGE_SIZE), copy_size);
    // TODO zero out the other part of the last page
    if(copy_from_user((void*) encl_page,
		      elf_usr_region + (k * PAGE_SIZE),
		      copy_size )){;
      ret = -EFAULT;
      break;
    }
  }
 
  return ret;
}

int keystone_app_load_elf(epm_t* epm, void* __user elf_usr_ptr, size_t len){
  int retval, error, i;
  struct elf_phdr eppnt;
  struct elfhdr elf_ex;
  struct elf_phdr* __user next_usr_phoff;
  unsigned long vaddr;
  unsigned long size = 0;
  
  error = -EFAULT;
  
  // TODO safety checks based on len
  if(copy_from_user(&elf_ex, elf_usr_ptr, sizeof(struct elfhdr)) != 0){
    goto out;
  }

  // check ELF header
  if(memcmp(elf_ex.e_ident, ELFMAG, SELFMAG) != 0)
    goto out;

  // Sanity check on elf type that its been linked as EXEC
  if(elf_ex.e_type != ET_EXEC || !elf_check_arch(&elf_ex))
    goto out;

  // Get each elf_phdr in order and deal with it
  next_usr_phoff = (struct elf_phdr* __user)((uintptr_t)elf_usr_ptr + elf_ex.e_phoff);
  for(i=0; i<elf_ex.e_phnum; i++, next_usr_phoff++) {

    // Copy next phdr
    if(copy_from_user(&eppnt, (void*)next_usr_phoff, sizeof(struct elf_phdr)) != 0){
      //bad
      continue;
    }

    // Create and copy
    if(eppnt.p_type != PT_LOAD) {
      pr_warn("keystone runtime includes an inconsistent program header\n");
      continue;
    }
    vaddr = eppnt.p_vaddr;
    //vaddr sanity check?
    size = eppnt.p_filesz;
    //pr_info("loading vaddr: %x, sz:%i\n",vaddr,size);

    retval = keystone_app_load_elf_region(epm,
					  elf_usr_ptr + eppnt.p_offset,
					  (void*)vaddr,
					  size);
    if(retval != 0){
      error = retval;
      break;
    }
  }

  error = 0;

  out:
    return error;
  
}


int keystone_rtld_init_app(enclave_t* enclave, void* __user app_ptr, size_t app_sz, size_t app_stack_sz, unsigned long stack_offset)
{
  unsigned long vaddr;
  int ret;
  epm_t* epm;
  epm = enclave->epm;

  /* setup enclave stack */
  for (vaddr = stack_offset - PAGE_UP(app_stack_sz); 
      vaddr < stack_offset; 
      vaddr += PAGE_SIZE) {
    epm_alloc_user_page_noexec(epm, vaddr);
  }

  // TODO fix eapp_sz so that its smaller, more accurate. right now its the whole elf
  ret = keystone_app_load_elf(epm, app_ptr, app_sz);
  return ret;
}

int keystone_rtld_init_runtime(enclave_t* enclave, void* __user rt_ptr, size_t rt_sz, unsigned long rt_stack_sz, unsigned long* rt_offset)
{
  epm_t* epm;
  int retval, error, i, j;
  struct elf_phdr *elf_phdata;
  struct elf_phdr *eppnt;
  struct elfhdr elf_ex;
  *rt_offset = -1UL;

  epm = enclave->epm;

  error = -ENOEXEC;
  if(copy_from_user(&elf_ex, rt_ptr, sizeof(struct elfhdr)) != 0){
    keystone_err("failed to read runtime header\n");
    goto out;
  }

  // check ELF header
  if(memcmp(elf_ex.e_ident, ELFMAG, SELFMAG) != 0) {
    keystone_err("invalid runtime executable\n");
    goto out;
  }

  // check runtime consistency
  if(elf_ex.e_type != ET_EXEC || !elf_check_arch(&elf_ex)) {
    keystone_err("invalid runtime executable\n");
    goto out;
  }

  j = sizeof(struct elf_phdr) * elf_ex.e_phnum;

  error = -ENOMEM;
  elf_phdata = kmalloc(j, GFP_KERNEL);
  if(!elf_phdata)
    goto out;

  eppnt = elf_phdata;

  error = -ENOEXEC;
  retval = copy_from_user(eppnt, rt_ptr + elf_ex.e_phoff, j);
  if(retval != 0) {
    keystone_err("failed to copy runtime phdr\n");
    goto out_free_ph;
  }
  
  for(eppnt = elf_phdata, i=0; i<elf_ex.e_phnum; eppnt++, i++) {
    unsigned long vaddr;
    unsigned long size = 0;

    if(eppnt->p_type != PT_LOAD) {
      keystone_warn("keystone runtime includes an inconsistent program header\n");
      continue;
    }
    vaddr = eppnt->p_vaddr;
    if(vaddr < *rt_offset) {
      *rt_offset = vaddr;
    }
    size = eppnt->p_filesz;
    if(size > eppnt->p_memsz) {
      pr_info("unexpected mismatch in elf program header: filesz %ld, memsz %llu\n", size, eppnt->p_memsz);
    }
    rtld_vm_mmap(epm, vaddr, size, rt_ptr, eppnt);
  }

  rtld_setup_stack(epm, -1UL, PAGE_UP(rt_stack_sz));

  error = 0;
out_free_ph:
  kfree(elf_phdata);
out:
  return error;
}

int keystone_rtld_init_untrusted(enclave_t* enclave, void* untrusted_va, size_t untrusted_size) 
{
  vaddr_t va;
  vaddr_t va_start = PAGE_DOWN((vaddr_t) untrusted_va);
  vaddr_t va_end = PAGE_UP((vaddr_t)untrusted_va + untrusted_size);

  if (va_start != (vaddr_t) untrusted_va)
    keystone_warn("shared buffer address is not aligned to PAGE_SIZE\n");

  for (va = va_start; va < va_end; va += PAGE_SIZE) 
  {
    utm_alloc_page(enclave->utm, enclave->epm, va, PTE_D | PTE_A | PTE_R | PTE_W );
  }
  return 0;
}

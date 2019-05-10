//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _KEYSTONE_H_
#define _KEYSTONE_H_

#include <asm/sbi.h>
#include <asm/csr.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/idr.h>

#include <linux/file.h>

/* IMPORTANT: This code assumes Sv39 */
#include "riscv64.h"

typedef uintptr_t vaddr_t;
typedef uintptr_t paddr_t;

extern struct miscdevice keystone_dev;
#define SBI_SM_CREATE_ENCLAVE   101
#define SBI_SM_DESTROY_ENCLAVE  102
#define SBI_SM_RUN_ENCLAVE      105
#define SBI_SM_STOP_ENCLAVE     106
#define SBI_SM_RESUME_ENCLAVE   107

/* error codes: need to add more */
#define ENCLAVE_INTERRUPTED     2

/* don't want to taint asm/sbi.h, so just copied SBI_CALL and increased # args */
#define _SBI_CALL(which, arg0, arg1, arg2, arg3, arg4, arg5) ({			\
	register uintptr_t a0 asm ("a0") = (uintptr_t)(arg0);	\
	register uintptr_t a1 asm ("a1") = (uintptr_t)(arg1);	\
	register uintptr_t a2 asm ("a2") = (uintptr_t)(arg2);	\
	register uintptr_t a3 asm ("a3") = (uintptr_t)(arg3);	\
	register uintptr_t a4 asm ("a4") = (uintptr_t)(arg4);	\
	register uintptr_t a5 asm ("a5") = (uintptr_t)(arg5);	\
	register uintptr_t a7 asm ("a7") = (uintptr_t)(which);	\
	asm volatile ("ecall"					\
		      : "+r" (a0)				\
		      : "r" (a1), "r" (a2), "r" (a3), "r" (a4), "r"(a5), "r" (a7)		\
		      : "memory");				\
	a0;							\
})

#define SBI_CALL_3(which, arg0, arg1, arg2) SBI_CALL(which,arg0, arg1, arg2)
#define SBI_CALL_4(which, arg0, arg1, arg2, arg3) _SBI_CALL(which, arg0, arg1, arg2, arg3, 0, 0)
#define SBI_CALL_5(which, arg0, arg1, arg2, arg3, arg4) _SBI_CALL(which, arg0, arg1, arg2, arg3, arg4, 0)
#define SBI_CALL_6(which, arg0, arg1, arg2, arg3, arg4, arg5) _SBI_CALL(which, arg0, arg1, arg2, arg3, arg4, arg5)

void keystone_handle_interrupts(void);

long keystone_ioctl(struct file* filep, unsigned int cmd, unsigned long arg);
int keystone_release(struct inode *inode, struct file *file);
int keystone_mmap(struct file *filp, struct vm_area_struct *vma);

struct free_page {
  vaddr_t vaddr;
  struct list_head freelist;
};

/* enclave private memory */
struct epm {
  struct list_head epm_free_list;
  pte_t* root_page_table;
  vaddr_t ptr;
  size_t size;
  unsigned long order;
  paddr_t pa;
  bool is_cma;
};

struct utm {
  struct list_head utm_free_list;
  pte_t* root_page_table;
  void* ptr;
  size_t size;
  unsigned long order;
};


struct enclave
{
  unsigned int eid;
  int close_on_pexit;
  struct utm* utm;
  struct epm* epm;
};


// global debug functions
void debug_dump(char* ptr, unsigned long size);

// runtime/app loader
int keystone_rtld_init_runtime(struct enclave* enclave, void* __user rt_ptr, size_t rt_sz, unsigned long rt_stack_sz, unsigned long* rt_offset);

int keystone_rtld_init_app(struct enclave* enclave, void* __user app_ptr, size_t app_sz, size_t app_stack_sz, unsigned long stack_offset);

// untrusted memory mapper
int keystone_rtld_init_untrusted(struct enclave* enclave, void* untrusted_ptr, size_t untrusted_size);

struct enclave* get_enclave_by_id(unsigned int ueid);
struct enclave* create_enclave(unsigned long min_pages);
int destroy_enclave(struct enclave* enclave);

unsigned int enclave_idr_alloc(struct enclave* enclave);
struct enclave* enclave_idr_remove(unsigned int ueid);
struct enclave* get_enclave_by_id(unsigned int ueid);

static inline uintptr_t  epm_satp(struct epm* epm) {
  return ((uintptr_t)epm->root_page_table >> RISCV_PGSHIFT | SATP_MODE_CHOICE);
}
void init_free_pages(struct list_head* pg_list, vaddr_t base, unsigned int count);
void put_free_page(struct list_head* pg_list, vaddr_t page_addr);
vaddr_t get_free_page(struct list_head* pg_list);

int epm_destroy(struct epm* epm);
int epm_init(struct epm* epm, unsigned int count);
int utm_destroy(struct utm* utm);
int utm_init(struct utm* utm, size_t untrusted_size);
int epm_clean_free_list(struct epm* epm);
int utm_clean_free_list(struct utm* utm);
paddr_t epm_va_to_pa(struct epm* epm, vaddr_t addr);
paddr_t epm_get_free_pa(struct epm* epm);
vaddr_t utm_alloc_page(struct utm* utm, struct epm* epm, vaddr_t addr, unsigned long flags);
size_t epm_alloc_vspace(struct epm* epm, vaddr_t addr, size_t num_pages);
vaddr_t epm_alloc_rt_page(struct epm* epm, vaddr_t addr);
vaddr_t epm_alloc_rt_page_noexec(struct epm* epm, vaddr_t addr);
vaddr_t epm_alloc_user_page(struct epm* epm, vaddr_t addr);
vaddr_t epm_alloc_user_page_noexec(struct epm* epm, vaddr_t addr);
void epm_free_page(struct epm* epm, vaddr_t addr);


unsigned long calculate_required_pages(
		unsigned long eapp_sz,
		unsigned long eapp_stack_sz,
		unsigned long rt_sz,
		unsigned long rt_stack_sz);

#define keystone_info(fmt, ...) \
  pr_info("keystone_enclave: " fmt, ##__VA_ARGS__)
#define keystone_err(fmt, ...) \
  pr_err("keystone_enclave: " fmt, ##__VA_ARGS__)
#define keystone_warn(fmt, ...) \
  pr_warn("keystone_enclave: " fmt, ##__VA_ARGS__)
#endif

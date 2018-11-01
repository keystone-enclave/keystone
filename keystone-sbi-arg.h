#ifndef _KEYSTONE_SBI_ARG_H_
#define _KEYSTONE_SBI_ARG_H_

struct keystone_sbi_pregion_t
{
  uintptr_t paddr;
  size_t size;
};

struct keystone_sbi_create_t
{
  // Memory regions for the enclave
  struct keystone_sbi_pregion_t epm_region;
  struct keystone_sbi_pregion_t copy_region;

  // Outputs from the creation process
  unsigned int* eid_pptr;
};


struct keystone_sbi_run_t
{
  unsigned int eid;
  uintptr_t entry_ptr;
  uintptr_t* ret_ptr;  
};

struct keystone_sbi_general_t
{
  unsigned int eid;
};


#endif

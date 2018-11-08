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
  struct keystone_sbi_pregion_t utm_region;

  // Configurations
  uintptr_t enclave_entry;
  uintptr_t runtime_entry;

  // Outputs from the creation process
  unsigned int* eid_pptr;
};

#endif

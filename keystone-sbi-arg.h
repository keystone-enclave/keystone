//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef _KEYSTONE_SBI_ARG_H_
#define _KEYSTONE_SBI_ARG_H_

#include "keystone_user.h"

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

  // physical addresses
  uintptr_t runtime_paddr;
  uintptr_t user_paddr;
  uintptr_t free_paddr;

  // Parameters
  struct runtime_params_t params;

  // Outputs from the creation process
  uint64_t* eid_vptr;
};

#endif

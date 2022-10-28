//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef sm_h
#define sm_h

#include <sbi/sbi_types.h>
#include "pmp.h"
#include "sm-sbi.h"
#include <sbi/riscv_encoding.h>

#define SMM_BASE  0x80000000
#define SMM_SIZE  0x200000

/* 0-1999 are not used (deprecated) */
#define FID_RANGE_DEPRECATED      1999
/* 2000-2999 are called by host */
#define SBI_SM_CREATE_ENCLAVE     2001
#define SBI_SM_DESTROY_ENCLAVE    2002
#define SBI_SM_RUN_ENCLAVE        2003
#define SBI_SM_RESUME_ENCLAVE     2005
#define FID_RANGE_HOST            2999
/* 3000-3999 are called by enclave */
#define SBI_SM_RANDOM             3001
#define SBI_SM_ATTEST_ENCLAVE     3002
#define SBI_SM_GET_SEALING_KEY    3003
#define SBI_SM_STOP_ENCLAVE       3004
#define SBI_SM_EXIT_ENCLAVE       3006
#define FID_RANGE_ENCLAVE         3999
/* 4000-4999 are experimental */
#define SBI_SM_CALL_PLUGIN        4000
#define FID_RANGE_CUSTOM          4999

/* error codes */
#define SBI_ERR_SM_ENCLAVE_SUCCESS                     0
#define SBI_ERR_SM_ENCLAVE_UNKNOWN_ERROR               100000
#define SBI_ERR_SM_ENCLAVE_INVALID_ID                  100001
#define SBI_ERR_SM_ENCLAVE_INTERRUPTED                 100002
#define SBI_ERR_SM_ENCLAVE_PMP_FAILURE                 100003
#define SBI_ERR_SM_ENCLAVE_NOT_RUNNABLE                100004
#define SBI_ERR_SM_ENCLAVE_NOT_DESTROYABLE             100005
#define SBI_ERR_SM_ENCLAVE_REGION_OVERLAPS             100006
#define SBI_ERR_SM_ENCLAVE_NOT_ACCESSIBLE              100007
#define SBI_ERR_SM_ENCLAVE_ILLEGAL_ARGUMENT            100008
#define SBI_ERR_SM_ENCLAVE_NOT_RUNNING                 100009
#define SBI_ERR_SM_ENCLAVE_NOT_RESUMABLE               100010
#define SBI_ERR_SM_ENCLAVE_EDGE_CALL_HOST              100011
#define SBI_ERR_SM_ENCLAVE_NOT_INITIALIZED             100012
#define SBI_ERR_SM_ENCLAVE_NO_FREE_RESOURCE            100013
#define SBI_ERR_SM_ENCLAVE_SBI_PROHIBITED              100014
#define SBI_ERR_SM_ENCLAVE_ILLEGAL_PTE                 100015
#define SBI_ERR_SM_ENCLAVE_NOT_FRESH                   100016
#define SBI_ERR_SM_DEPRECATED                          100099
#define SBI_ERR_SM_NOT_IMPLEMENTED                     100100

#define SBI_ERR_SM_PMP_SUCCESS                         0
#define SBI_ERR_SM_PMP_REGION_SIZE_INVALID             100020
#define SBI_ERR_SM_PMP_REGION_NOT_PAGE_GRANULARITY     100021
#define SBI_ERR_SM_PMP_REGION_NOT_ALIGNED              100022
#define SBI_ERR_SM_PMP_REGION_MAX_REACHED              100023
#define SBI_ERR_SM_PMP_REGION_INVALID                  100024
#define SBI_ERR_SM_PMP_REGION_OVERLAP                  100025
#define SBI_ERR_SM_PMP_REGION_IMPOSSIBLE_TOR           100026

void sm_init(bool cold_boot);

/* platform specific functions */
#define ATTESTATION_KEY_LENGTH  64
void sm_retrieve_pubkey(void* dest);
void sm_sign(void* sign, const void* data, size_t len);
int sm_derive_sealing_key(unsigned char *key,
                          const unsigned char *key_ident,
                          size_t key_ident_size,
                          const unsigned char *enclave_hash);

/* creation parameters */
struct keystone_sbi_pregion
{
  uintptr_t paddr;
  size_t size;
};
struct runtime_va_params_t
{
  uintptr_t runtime_entry;
  uintptr_t user_entry;
  uintptr_t untrusted_ptr;
  uintptr_t untrusted_size;
};

struct runtime_pa_params
{
  uintptr_t dram_base;
  uintptr_t dram_size;
  uintptr_t runtime_base;
  uintptr_t user_base;
  uintptr_t free_base;
};

struct keystone_sbi_create
{
  struct keystone_sbi_pregion epm_region;
  struct keystone_sbi_pregion utm_region;

  uintptr_t runtime_paddr;
  uintptr_t user_paddr;
  uintptr_t free_paddr;

  struct runtime_va_params_t params;
  unsigned int* eid_pptr;
};

int osm_pmp_set(uint8_t perm);
#endif

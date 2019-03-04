//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#ifndef sm_h
#define sm_h

#include <stdint.h>
#include "pmp.h"
#include "sm-sbi.h"
#include "encoding.h"

#define SMM_BASE  0x80000000
#define SMM_SIZE  0x200000

#define SBI_SM_CREATE_ENCLAVE   101
#define SBI_SM_DESTROY_ENCLAVE  102
#define SBI_SM_ATTEST_ENCLAVE   103
#define SBI_SM_RUN_ENCLAVE      105
#define SBI_SM_STOP_ENCLAVE     106
#define SBI_SM_RESUME_ENCLAVE   107
#define SBI_SM_EXIT_ENCLAVE     1101
#define SBI_SM_NOT_IMPLEMENTED  1111

/* error codes */
#define ENCLAVE_NO_FREE_RESOURCE            (enclave_ret_t)-3U
#define ENCLAVE_NOT_IMPLEMENTED             (enclave_ret_t)-2U
#define ENCLAVE_UNKNOWN_ERROR               (enclave_ret_t)-1U
#define ENCLAVE_SUCCESS                     (enclave_ret_t)0
#define ENCLAVE_INVALID_ID                  (enclave_ret_t)1
#define ENCLAVE_INTERRUPTED                 (enclave_ret_t)2
#define ENCLAVE_PMP_FAILURE                 (enclave_ret_t)3
#define ENCLAVE_NOT_RUNNABLE                (enclave_ret_t)4
#define ENCLAVE_NOT_DESTROYABLE             (enclave_ret_t)5
#define ENCLAVE_REGION_OVERLAPS             (enclave_ret_t)6
#define ENCLAVE_NOT_ACCESSIBLE              (enclave_ret_t)7
#define ENCLAVE_ILLEGAL_ARGUMENT            (enclave_ret_t)8
#define ENCLAVE_NOT_RUNNING                 (enclave_ret_t)9
#define ENCLAVE_NOT_RESUMABLE               (enclave_ret_t)10
#define ENCLAVE_EDGE_CALL_HOST              (enclave_ret_t)11
#define ENCLAVE_NOT_INITIALIZED             (enclave_ret_t)12

#define PMP_UNKNOWN_ERROR                   -1U
#define PMP_SUCCESS                         0
#define PMP_REGION_SIZE_INVALID             20
#define PMP_REGION_NOT_PAGE_GRANULARITY     21
#define PMP_REGION_NOT_ALIGNED              22
#define PMP_REGION_MAX_REACHED              23
#define PMP_REGION_INVALID                  24
#define PMP_REGION_OVERLAP                  25

#define STOP_TIMER_INTERRUPT  0
#define STOP_EDGE_CALL_HOST   1

void sm_init(void);

/* platform specific functions */
#define ATTESTATION_KEY_LENGTH  64
void sm_retrieve_pubkey(void* dest);
void sm_sign(void* sign, const void* data, size_t len);

/* creation parameters */
struct keystone_sbi_pregion_t
{
  uintptr_t paddr;
  size_t size;
};
struct runtime_params_t
{
  uint64_t runtime_entry;
  uint64_t user_entry;
  uint64_t untrusted_ptr;
  uint64_t untrusted_size;
};
struct keystone_sbi_create_t
{
  struct keystone_sbi_pregion_t epm_region;
  struct keystone_sbi_pregion_t utm_region;
  struct runtime_params_t params;
  unsigned int* eid_pptr;
};

int osm_pmp_set(uint8_t perm);
#endif

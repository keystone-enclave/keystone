#ifndef __SM_CALL_H__
#define __SM_CALL_H__

// BKE (Berkeley Keystone Enclave)
#define SBI_EXT_EXPERIMENTAL_KEYSTONE_ENCLAVE 0x08424b45

#define SBI_SET_TIMER 0
#define SBI_CONSOLE_PUTCHAR 1
#define SBI_CONSOLE_GETCHAR 2

/* 0-1999 are not used (deprecated) */
#define FID_RANGE_DEPRECATED      1999
/* 2000-2999 are called by host */
#define SBI_SM_CREATE_ENCLAVE    2001
#define SBI_SM_DESTROY_ENCLAVE   2002
#define SBI_SM_RUN_ENCLAVE       2003
#define SBI_SM_RESUME_ENCLAVE    2005
#define FID_RANGE_HOST           2999

/* 3000-3999 are called by enclave */
#define SBI_SM_RANDOM            3001
#define SBI_SM_ATTEST_ENCLAVE    3002
#define SBI_SM_GET_SEALING_KEY   3003
#define SBI_SM_STOP_ENCLAVE      3004
#define SBI_SM_EXIT_ENCLAVE      3006
#define FID_RANGE_ENCLAVE        3999

/* 4000-4999 are experimental */
#define SBI_SM_CALL_PLUGIN        4000
#define FID_RANGE_CUSTOM          4999

/* Plugin IDs and Call IDs */
#define SM_MULTIMEM_PLUGIN_ID   0x01
#define SM_MULTIMEM_CALL_GET_SIZE 0x01
#define SM_MULTIMEM_CALL_GET_ADDR 0x02

/* Enclave stop reasons requested */
#define STOP_TIMER_INTERRUPT  0
#define STOP_EDGE_CALL_HOST   1
#define STOP_EXIT_ENCLAVE     2

/* Structs for interfacing into the SM */
struct runtime_params_t {
  uintptr_t dram_base;
  uintptr_t dram_size;
  uintptr_t start_pc;
  uintptr_t free_base;
  uintptr_t untrusted_base;
  uintptr_t untrusted_size;
  uintptr_t free_requested; // for attestation
};

struct keystone_sbi_pregion_t {
  uintptr_t paddr;
  size_t size;
};

struct keystone_sbi_create_t {
  struct keystone_sbi_pregion_t epm_region;
  struct keystone_sbi_pregion_t utm_region;
  uintptr_t free_offset;
};

// TODO(Evgeny): how do we ensure no compiler re-ordering?
#define MSR_NAME_LEN 64
// #include "common/sha3.h" // TODO(Evgeny): fix the include
#define MDSIZE 64
#define MSR_START_FILENAME "__0start"
#define MSR_RUNTIME_FILENAME "__1runtime"
#define MSR_EAPP_FILENAME "__2eapp"
#define MSR_FREE_MEM "free_mem"
#define MSR_UT_MEM "ut_mem"
typedef struct {
  char name[MSR_NAME_LEN];
  uintptr_t type;
  char hash[MDSIZE];
} resource_hash_t;

typedef struct {
  char name[MSR_NAME_LEN];
  uintptr_t type;
  uintptr_t offset;
  uintptr_t size;
} resource_ptr_t;

typedef struct {
  char name[MSR_NAME_LEN];
  uintptr_t val;
} runtime_val_t;

// TODO(Evgeny): a way to make this more convenient?
typedef struct {
  uintptr_t runtime_arr, id_res_arr,
    id_abs_arr, res_arr, abs_arr, pad_start;
  // resource_value_t runtime_values[];
  // resource_ptr_t identity_resident[];
  // resource_hash_t identity_absent[];
  // resource_ptr_t resident[];
  // resource_hash_t absent[];
  // byte pad_start[];
} enclave_bundle_header_t;

#endif  // __SM_CALL_H__

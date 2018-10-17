#ifndef sm_h
#define sm_h

//FIXME: just arbitrary 128MB range
#include <stdint.h>
#include "pmp.h"
#include "sm-sbi.h"
#include "encoding.h"

#define SMM_BASE  0x80000000
#define SMM_SIZE  0x200000

#define SBI_SM_CREATE_ENCLAVE   101
#define SBI_SM_DESTROY_ENCLAVE  102
#define SBI_SM_RUN_ENCLAVE      105
#define SBI_SM_STOP_ENCLAVE     106
#define SBI_SM_RESUME_ENCLAVE   107
#define SBI_SM_EXIT_ENCLAVE     1101
#define SBI_SM_NOT_IMPLEMENTED  1111

/* error codes */
#define ENCLAVE_NOT_IMPLEMENTED             -2U
#define ENCLAVE_UNKNOWN_ERROR               -1U
#define ENCLAVE_SUCCESS                     0
#define ENCLAVE_INVALID_ID                  1
#define ENCLAVE_INTERRUPTED                 2
#define ENCLAVE_PMP_FAILURE                 3
#define ENCLAVE_NOT_RUNNABLE                4
#define ENCLAVE_NOT_DESTROYABLE             5
#define ENCLAVE_REGION_OVERLAPS             6
#define ENCLAVE_NOT_ACCESSIBLE              7
#define ENCLAVE_ILLEGAL_ARGUMENT            8
#define ENCLAVE_NOT_RUNNING                 9

#define PMP_UNKNOWN_ERROR                   -1U
#define PMP_SUCCESS                         0
#define PMP_REGION_SIZE_INVALID             10
#define PMP_REGION_NOT_PAGE_GRANULARITY     11
#define PMP_REGION_NOT_ALIGNED              12
#define PMP_REGION_MAX_REACHED              13
#define PMP_REGION_INVALID                  14

void sm_init(void);

#endif

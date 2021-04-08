#ifndef __SM_MULTIMEM_H__
#define __SM_MULTIMEM_H__

#include "plugins/plugins.h"
#include "enclave.h"

#define MULTIMEM_GET_OTHER_REGION_SIZE 0x1
#define MULTIMEM_GET_OTHER_REGION_ADDR 0x2

uintptr_t do_sbi_multimem(enclave_id id, uintptr_t call_id, uintptr_t arg0);

#endif

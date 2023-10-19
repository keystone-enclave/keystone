//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "enclave.h"
#include "crypto.h"
#include "page.h"
#include <sbi/sbi_console.h>

unsigned long hash_enclave(struct enclave* enclave){
  hash_ctx hash_ctx;
  hash_init(&hash_ctx);

  struct runtime_params* params = &(enclave->params);

  // hash memory size
  uintptr_t sizes[2] = {params->untrusted_size, params->free_requested};
  hash_extend(&hash_ctx, sizes, sizeof(sizes));

  // hash the epm contents
  // using pointers to ensure that they themselves are correct
  uintptr_t loader = params->dram_base;
  uintptr_t runtime = params->runtime_base;
  uintptr_t eapp = params->user_base;
  uintptr_t free = params->free_base;
  for (uintptr_t page = loader; page < runtime; page += RISCV_PGSIZE) {
    hash_extend_page(&hash_ctx, (void*) page);
  }
  for (uintptr_t page = runtime; page < eapp; page += RISCV_PGSIZE) {
    hash_extend_page(&hash_ctx, (void*) page);
  }
  for (uintptr_t page = eapp; page < free; page += RISCV_PGSIZE) {
    hash_extend_page(&hash_ctx, (void*) page);
  }

  hash_finalize(enclave->hash, &hash_ctx);

  return SBI_ERR_SM_ENCLAVE_SUCCESS;
}

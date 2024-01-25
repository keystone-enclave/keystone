//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "enclave.h"
#include <crypto.h>
#include "page.h"
#include <sbi/sbi_console.h>

/* This will hash the loader and the runtime + eapp elf files. */
static int validate_and_hash_epm(hash_ctx* ctx, struct enclave* encl)
{
  uintptr_t loader = encl->params.dram_base; // also base
  uintptr_t runtime = encl->params.runtime_base;
  uintptr_t eapp = encl->params.user_base;
  uintptr_t free = encl->params.free_base;

  // ensure pointers don't point to middle of correct files
  uintptr_t sizes[3] = {runtime - loader, eapp - runtime, free - eapp};
  hash_extend(ctx, (void*) sizes, sizeof(sizes));

  // using pointers to ensure that they themselves are correct
  // TODO(Evgeny): can extend by entire file instead of page at a time?
  for (uintptr_t page = loader; page < runtime; page += RISCV_PGSIZE) {
    hash_extend_page(ctx, (void*) page);
  }
  for (uintptr_t page = runtime; page < eapp; page += RISCV_PGSIZE) {
    hash_extend_page(ctx, (void*) page);
  }
  for (uintptr_t page = eapp; page < free; page += RISCV_PGSIZE) {
    hash_extend_page(ctx, (void*) page);
  }
  return 0;
}

unsigned long validate_and_hash_enclave(struct enclave* enclave){
  hash_ctx ctx;
  hash_init(&ctx);

  // TODO: ensure untrusted and free sizes

  // hash the epm contents
  int valid = validate_and_hash_epm(&ctx, enclave);

  if(valid == -1){
    return SBI_ERR_SM_ENCLAVE_ILLEGAL_PTE;
  }

  hash_finalize(enclave->hash, &ctx);

  return SBI_ERR_SM_ENCLAVE_SUCCESS;
}

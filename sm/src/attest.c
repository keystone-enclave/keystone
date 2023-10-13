//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "enclave.h"
#include "crypto.h"
#include "page.h"
#include <sbi/sbi_console.h>

typedef uintptr_t pte_t;
/* This will hash the loader and the runtime + eapp elf files. */
int validate_and_hash_epm(hash_ctx* hash_ctx, struct enclave* encl)
{
  pte_t* walk;
  int i;

  #ifdef this_is_not_defined
  // $a1: (PA) DRAM base,
  regs->a1 = (uintptr_t) enclaves[eid].pa_params.dram_base;
  // $a2: (PA) DRAM size,
  regs->a2 = (uintptr_t) enclaves[eid].pa_params.dram_size;
  // $a3: (PA) kernel location,
  regs->a3 = (uintptr_t) enclaves[eid].pa_params.runtime_base;
  // $a4: (PA) user location,
  regs->a4 = (uintptr_t) enclaves[eid].pa_params.user_base;
  // $a5: (PA) freemem location,
  regs->a5 = (uintptr_t) enclaves[eid].pa_params.free_base;
  // $a6: (VA) utm base,
  regs->a6 = (uintptr_t) enclaves[eid].params.untrusted_ptr;
  // $a7: (size_t) utm size
  regs->a7 = (uintptr_t) enclaves[eid].params.untrusted_size;
  #endif

  uintptr_t loader = encl->pa_params.dram_base; // also base
  uintptr_t runtime = encl->pa_params.runtime_base;
  uintptr_t eapp = encl->pa_params.user_base;
  uintptr_t free = encl->pa_params.free_base;
  uintptr_t untrusted = encl->params.untrusted_ptr;
  uintptr_t end = loader + encl->pa_params.dram_size;

  // using pointers to ensure that they themselves are correct
  if (runtime < loader || runtime > end) {
    return -1;
  }
  for (uintptr_t page = loader; page < runtime; page += RISCV_PGSIZE) {
    hash_extend_page(hash_ctx, (void*) page);
  }
  if (eapp < runtime || eapp > end) {
    return -1;
  }
  for (uintptr_t page = runtime; page < eapp; page += RISCV_PGSIZE) {
    hash_extend_page(hash_ctx, (void*) page);
  }
  if (free < eapp || free > end) {
    return -1;
  }
  for (uintptr_t page = eapp; page < free; page += RISCV_PGSIZE) {
    hash_extend_page(hash_ctx, (void*) page);
  }
}

unsigned long validate_and_hash_enclave(struct enclave* enclave){
  hash_ctx hash_ctx;
  hash_init(&hash_ctx);

  // TODO: move untrusted_ptr to pa_params and remove below !!!!!! also what to do w/ this?
  // hash the runtime parameters
  // hash_extend(&hash_ctx, &enclave->params, sizeof(struct runtime_va_params_t));

  // hash the epm contents
  int valid = validate_and_hash_epm(&hash_ctx, enclave);

  if(valid == -1){
    return SBI_ERR_SM_ENCLAVE_ILLEGAL_PTE;
  }

  hash_finalize(enclave->hash, &hash_ctx);

  return SBI_ERR_SM_ENCLAVE_SUCCESS;
}

//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "enclave.h"
#include <crypto.h>
#include "page.h"
#include <sbi/sbi_console.h>

int measure_res_arr(resource_hash_t* elem, resource_hash_t* end, hash_ctx* hash_ctx) {
  for (; elem < end; elem++) {
    hash_extend(hash_ctx, elem, sizeof(elem[0]));
  }
  return 0;
}

int measure_res_arr(resource_ptr_t* elem, resource_ptr_t* end, hash_ctx* hash_ctx, void* limit) {
    for (; elem < end; elem++) {
        hash_extend(hash_ctx, elem->name, sizeof(elem->name));
        hash_extend(hash_ctx, elem->type, sizeof(elem->type));
        if (elem->offset + elem->size >= limit) {
            return 1;
        }
        hash_extend(hash_ctx, (void*) elem->offset, elem->size); // TODO: is > page ok?
    }
    return 0;
}

unsigned long validate_and_hash_enclave(struct enclave* enclave){
  // TODO(Evgeny): move discovery into enclave
  uintptr_t ebase = enclave->params.dram_base;
  enclave_bundle_header_t* ebundle_h = (enclave_bundle_header_t*) ebase;
  for (resource_ptr_t* id_res_resource = ebundle_h->id_res_arr; id_res_resource < ebundle_h->id_abs_arr; id_res_resource++) {
    if (strcmp(id_res_resource->name, "loader") == 0) {
      enclave->params.loader_base = ebase + id_res_resource->offset;
    }
    if (strcmp(id_res_resource->name, "runtime") == 0) {
      enclave->params.runtime_base = ebase + id_res_resource->offset;
    }
    if (strcmp(id_res_resource->name, "eapp") == 0) {
      enclave->params.user_base = ebase + id_res_resource->offset;
    }
  }
  if (!enclave->params.loader_base || !enclave->params.runtime_base || !enclave->params.user_base) {
    return SBI_ERR_SM_ENCLAVE_ILLEGAL_PTE;
  }
  void* free = (void *) enclave->params.free_base;

  // TODO: ensure untrusted and free sizes

  // hash the epm contents
  hash_ctx ctx;
  hash_init(&ctx);
  bool fail_state = 0;
  // fail_state |= measure_res_arr((void *) ebase + ebundle_h->runtime_arr, (void *) ebase + ebundle_h->id_res_arr, &ctx);
  fail_state |= measure_res_arr(ebase + ebundle_h->id_res_arr, ebase + ebundle_h->id_abs_arr, &ctx, free);
  fail_state |= measure_res_arr(ebase + ebundle_h->id_abs_arr, ebase + ebundle_h->res_arr, &ctx);
  hash_ctx ctx_copy = ctx;
  hash_finalize(enclave->identity, &ctx_copy);
  fail_state |= measure_res_arr(ebase + ebundle_h->res_arr, ebase + ebundle_h->abs_arr, &ctx, free);
  fail_state |= measure_res_arr(ebase + ebundle_h->abs_arr, ebase + ebundle_h->data, &ctx);
  if (fail_state) {
      return SBI_ERR_SM_ENCLAVE_ILLEGAL_PTE;
  }
  hash_finalize(enclave->hash, &ctx);

  return SBI_ERR_SM_ENCLAVE_SUCCESS;
}

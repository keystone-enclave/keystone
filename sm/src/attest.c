//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "enclave.h"
#include <crypto.h>
#include "page.h"
#include <sbi/sbi_console.h>
// #include <sbi/sbi_string.h> // TODO(Evgeny): try to replace manual strcmp

static int strcmp(const char* left, const char* right) {
  for (; *left != '\0'; left++, right++) {
    if (*left != *right) {
      return (*left - *right);
    }
    if (*right == '\0') {
      return -1;
    }
  }
  if (*right != '\0') {
    return 1;
  }
  return 0;
}

static int validate_arr(uintptr_t ebase, uintptr_t efilled_size, uintptr_t start_off, uintptr_t end_off, uintptr_t elem_size) {
  uintptr_t start = ebase + start_off;
  uintptr_t end = ebase + end_off;
  if (start_off > end_off // bad loop condition
    || end_off > efilled_size // out of bounds reads
    || ebase >= start || ebase >= end // overflow
    || (end - start) % elem_size != 0 ) { // hanging read
      return 1;
  }
  return 0;
}

static int measure_absent_arr(uintptr_t ebase, uintptr_t efilled_size, uintptr_t start_off, uintptr_t end_off, hash_ctx* ctx) {
  if(validate_arr(ebase, efilled_size, start_off, end_off, sizeof(resource_hash_t))) {
    return 1;
  }
  resource_hash_t* elem = (resource_hash_t*) (ebase + start_off);
  resource_hash_t* end = (resource_hash_t*) (ebase + end_off);
  for (; elem < end; elem++) {
    hash_extend(ctx, (void*) elem, sizeof(resource_hash_t));
  }
  return 0;
}

static int measure_resident_arr(uintptr_t ebase, uintptr_t efilled_size, uintptr_t start_off, uintptr_t end_off, hash_ctx* ctx) {
  if(validate_arr(ebase, efilled_size, start_off, end_off, sizeof(resource_ptr_t))) {
    return 1;
  }
  resource_ptr_t* elem = (resource_ptr_t*) (ebase + start_off);
  resource_ptr_t* end = (resource_ptr_t*) (ebase + end_off);
  hash_ctx file_ctx;
  char file_hash[MDSIZE];
  for (; elem < end; elem++) {
      hash_extend(ctx, (char*) elem->name, sizeof(elem->name));
      hash_extend(ctx, (char*) &elem->type, sizeof(elem->type));
      if (elem->offset >= elem->offset + elem->size
        || elem->offset + elem->size > efilled_size) {
          return 1;
      }
      // hash file
      hash_init(&file_ctx);
      hash_extend(&file_ctx, (char*) (ebase + elem->offset), elem->size);
      hash_finalize(file_hash, &file_ctx);
      hash_extend(ctx, file_hash, sizeof(file_hash));
  }
  return 0;
}

static int measure_runtime_arr(uintptr_t ebase, uintptr_t efilled_size, uintptr_t start_off, uintptr_t end_off, hash_ctx* ctx) {
  if(validate_arr(ebase, efilled_size, start_off, end_off, sizeof(runtime_val_t))) {
    return 1;
  }
  runtime_val_t* elem = (runtime_val_t*) (ebase + start_off);
  runtime_val_t* end = (runtime_val_t*) (ebase + end_off);
  for (; elem < end; elem++) {
    hash_extend(ctx, (void*) elem, sizeof(runtime_val_t));
  }
  return 0;
}

unsigned long validate_and_hash_enclave(struct enclave* enclave){
  uintptr_t ebase = enclave->params.dram_base;
  enclave_bundle_header_t* ebundle_h = (enclave_bundle_header_t*) ebase;
  uintptr_t efilled_size = enclave->params.free_base - ebase;

  // hash the epm contents
  hash_ctx ctx;
  hash_init(&ctx);
  bool fail_state = 0;
  fail_state |= measure_runtime_arr(ebase, efilled_size, ebundle_h->runtime_arr, ebundle_h->id_res_arr, &ctx);
  fail_state |= measure_resident_arr(ebase, efilled_size, ebundle_h->id_res_arr, ebundle_h->id_abs_arr, &ctx);
  fail_state |= measure_absent_arr(ebase, efilled_size, ebundle_h->id_abs_arr, ebundle_h->res_arr, &ctx);
  hash_ctx ctx_copy = ctx;
  fail_state |= measure_resident_arr(ebase, efilled_size, ebundle_h->res_arr, ebundle_h->abs_arr, &ctx);
  fail_state |= measure_absent_arr(ebase, efilled_size, ebundle_h->abs_arr, ebundle_h->pad_start, &ctx);
  if (fail_state) {
      return SBI_ERR_SM_ENCLAVE_ILLEGAL_ARGUMENT;
  }

  // confirm the runtime value claims
  runtime_val_t* runtime_val = (runtime_val_t*) (ebase + ebundle_h->runtime_arr);
  resource_ptr_t* id_res_resource = (resource_ptr_t*) (ebase + ebundle_h->id_res_arr);
  for (; runtime_val < (runtime_val_t*) id_res_resource; runtime_val++) {
    if (strcmp(runtime_val->name, MSR_FREE_MEM) == 0) {
      if (runtime_val->val > enclave->params.dram_size - enclave->params.free_base) {
        return SBI_ERR_SM_ENCLAVE_ILLEGAL_ARGUMENT;
      }
    } else if (strcmp(runtime_val->name, MSR_UT_MEM) == 0) {
      if (runtime_val->val != enclave->params.untrusted_size) {
        return SBI_ERR_SM_ENCLAVE_ILLEGAL_ARGUMENT;
      }
    } else {
      // claim unsupported by platform
      return SBI_ERR_SM_ENCLAVE_ILLEGAL_ARGUMENT;
    }
  }

  resource_ptr_t* id_abs_arr = (resource_ptr_t*) (ebase + ebundle_h->id_abs_arr);
  // note: no overflow/ out of bounds possible because measure_resident_arr would have failed
  for (; id_res_resource < id_abs_arr; id_res_resource++) {
    if (strcmp(id_res_resource->name, MSR_START_FILENAME) == 0) {
      enclave->params.start_pc = ebase + id_res_resource->offset;
      break;
    }
  }
  if (!enclave->params.start_pc) {
    return SBI_ERR_SM_ENCLAVE_ILLEGAL_ARGUMENT;
  }

  hash_finalize(enclave->identity, &ctx_copy);
  hash_finalize(enclave->hash, &ctx);

  return SBI_ERR_SM_ENCLAVE_SUCCESS;
}

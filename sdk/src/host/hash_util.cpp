//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
extern "C" {
#include "common/sha3.h"
}
#include "Memory.hpp"
#include "hash_util.hpp"

void
hash_init(hash_ctx_t* hash_ctx) {
  sha3_init(hash_ctx, MDSIZE);
}

void
hash_extend(hash_ctx_t* hash_ctx, const void* ptr, size_t len) {
  sha3_update(hash_ctx, ptr, len);
}

void
hash_extend_page(hash_ctx_t* hash_ctx, const void* ptr) {
  sha3_update(hash_ctx, ptr, RISCV_PGSIZE);
}

void
hash_finalize(void* md, hash_ctx_t* hash_ctx) {
  sha3_final(md, hash_ctx);
}

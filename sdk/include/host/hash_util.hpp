//******************************************************************************
// Copyright (c) 2020, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#pragma once

extern "C" {
#include "common/sha3.h"
}

typedef sha3_ctx_t hash_ctx_t;

void
hash_init(hash_ctx_t* hash_ctx);
void
hash_extend(hash_ctx_t* hash_ctx, const void* ptr, size_t len);
void
hash_extend_page(hash_ctx_t* hash_ctx, const void* ptr);
void
hash_finalize(void* md, hash_ctx_t* hash_ctx);

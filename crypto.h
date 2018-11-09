#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include "sha3/sha3.h"
#include <stdint.h>

typedef sha3_ctx_t hash_ctx_t;
#define MDSIZE  64

void hash_init(hash_ctx_t* hash_ctx);
void hash_extend(hash_ctx_t* hash_ctx, const void* ptr, size_t len);
void hash_extend_page(hash_ctx_t* hash_ctx, const void* ptr);
void hash_finalize(void* md, hash_ctx_t* hash_ctx);

#endif /* crypto.h */

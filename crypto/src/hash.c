#include "crypto/hash.h"

/* Wrappers around internal functions. */

static int sha3_init_wrapper(crypto_hash_ctx_variant *ctx) {
  return sha3_init(&ctx->sha3_ctx, CRYPTO_MD_SIZE_SHA3);
}

static int sha3_update_wrapper(crypto_hash_ctx_variant *ctx,
                               const void *data, unsigned int len) {
  return sha3_update(&ctx->sha3_ctx, data, len);
}

static int sha3_final_wrapper(crypto_hash_ctx_variant *ctx,
                              unsigned char *md) {
  return sha3_final(md, &ctx->sha3_ctx);
}

/* The SHA256 implementation we're using doesn't provide success flags, so we
   set trusted_return = 0 and return 1 for each function. */

static int sha256_init_wrapper(crypto_hash_ctx_variant *ctx) {
  sha256_init(&ctx->sha256_ctx);
  return 1;
}

static int sha256_update_wrapper(crypto_hash_ctx_variant *ctx,
                                 const void *data, unsigned int len) {
  sha256_update(&ctx->sha256_ctx, data, len);
  return 1;
}

static int sha256_final_wrapper(crypto_hash_ctx_variant *ctx,
                                unsigned char *md) {
  sha256_final(&ctx->sha256_ctx, md);
  return 1;
}

/* Definitions. */

typedef int (*hash_init_strategy)(crypto_hash_ctx_variant *ctx);
typedef int (*hash_update_strategy)(crypto_hash_ctx_variant *ctx,
                                    const void *data, unsigned int len);
typedef int (*hash_final_strategy)(crypto_hash_ctx_variant *ctx,
                                   unsigned char *md);

typedef struct {
  hash_init_strategy init;
  hash_update_strategy update;
  hash_final_strategy final;
  unsigned int digest_size;
  int trusted_return;
} crypto_hash_def;

static const crypto_hash_def hash_tbl[2] = {
    [CRYPTO_SHA3] = {.init = &sha3_init_wrapper,
                     .update = &sha3_update_wrapper,
                     .final = &sha3_final_wrapper,
                     .digest_size = CRYPTO_MD_SIZE_SHA3,
                     .trusted_return = 1},
    [CRYPTO_SHA256] = {.init = &sha256_init_wrapper,
                       .update = &sha256_update_wrapper,
                       .final = &sha256_final_wrapper,
                       .digest_size = CRYPTO_MD_SIZE_SHA256,
                       .trusted_return = 0}};

/* Interface functions. */

int crypto_hash_init(crypto_hash *hash, crypto_hash_algorithm_t alg) {
  if (!hash) {
    return 0;
  }
  hash->type = alg;
  return hash_tbl[alg].init(&hash->_ctx);
}

int crypto_hash_update(crypto_hash *hash, const void *data, unsigned int len) {
  return hash ? hash_tbl[hash->type].update(&hash->_ctx, data, len) : 0;
}

int crypto_hash_final(crypto_hash *hash, unsigned char *md) {
  return hash ? (hash_tbl[hash->type]).final(&hash->_ctx, md) : 0;
}

unsigned int crypto_hash_digest_size(crypto_hash *hash) {
  return hash ? (hash_tbl[hash->type]).digest_size : 0;
}

int crypto_hash_has_trusted_return(crypto_hash *hash) {
  return hash ? (hash_tbl[hash->type]).trusted_return : 0;
}

unsigned char* crypto_hash_data(const void *data, unsigned int data_len,
                       unsigned char *md, crypto_hash_algorithm_t alg) {
  if (!(data && data_len && md)) {
    return NULL;
  }
  
  crypto_hash hash;
  crypto_hash_init(&hash, alg);
  crypto_hash_update(&hash, data, data_len);
  crypto_hash_final(&hash, md);

  return md;
}

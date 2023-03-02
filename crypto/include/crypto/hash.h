#ifndef CRYPTO_HASH_H_
#define CRYPTO_HASH_H_

#include "sha256.h"
#include "sha3.h"

typedef union {
  sha3_ctx_t sha3_ctx;
  SHA256_CTX sha256_ctx;
} hash_ctx_variant;

typedef enum {
  CRYPTO_SHA3 = 0,
  CRYPTO_SHA256
} hash_algorithm_t;


#define CRYPTO_HASH_MD_SIZE_SHA3 64
#define CRYPTO_HASH_MD_SIZE_SHA256 32

typedef struct {
  /* The internal context. */
  hash_ctx_variant _ctx;
  hash_algorithm_t type;
} crypto_hash;

int crypto_hash_init(crypto_hash *hash, hash_algorithm_t alg);
int crypto_hash_update(crypto_hash *hash, const void *data, unsigned int len);
int crypto_hash_final(crypto_hash *hash, unsigned char *md);
unsigned int crypto_hash_digest_size(crypto_hash *hash);
int crypto_hash_has_trusted_return(crypto_hash *hash);

#endif /* CRYPTO_HASH_H_ */

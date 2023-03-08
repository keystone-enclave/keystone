#ifndef CRYPTO_HASH_H_
#define CRYPTO_HASH_H_

#include "sha256.h"

typedef SHA256_CTX crypto_sha256_ctx;
#define CRYPTO_SHA256_DIGEST_SIZE 32

/* We provide thin wrappers over these functions in case we'd like to change the
   underyling SHA-256 implementation later. */

/* Initialize the context, CTX. This must be called before the hash is updated
   or finalized. */
void crypto_sha256_init(crypto_sha256_ctx *ctx);

/* Update the hash with the specified DATA. */
void crypto_sha256_update(crypto_sha256_ctx *ctx, const void *data,
                          unsigned int data_len);

/* Finalize the hash and store it in MESSAGE_DIGEST, which must be large enough
   to store the hash. For SHA-256, this is 32 bytes (CRYPTO_SHA256_DIGEST_SIZE). */
void crypto_sha256_final(crypto_sha256_ctx *ctx, unsigned char *message_digest);

/* Hash the specified DATA, storing the hash in MESSAGE_DIGEST, which must be
   large enough to store the hash. For SHA-256, this is 32 bytes
   (CRYPTO_SHA256_DIGEST_SIZE). */
unsigned char *crypto_sha256_hash_data(const void *data, unsigned int data_len,
                                       unsigned char *message_digest);

#endif /* CRYPTO_HASH_H_ */

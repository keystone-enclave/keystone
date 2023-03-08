#include "crypto/hash.h"


void crypto_sha256_init(crypto_sha256_ctx *ctx) {
  sha256_init(ctx);
}
void crypto_sha256_update(crypto_sha256_ctx *ctx, const void *data,
                          unsigned int data_len) {
  sha256_update(ctx, data, data_len);
}
void crypto_sha256_final(crypto_sha256_ctx *ctx, unsigned char* message_digest) {
  sha256_final(ctx, message_digest);
}

unsigned char *crypto_sha256_hash_data(const void *data, unsigned int data_len,
                                       unsigned char *message_digest) {
  crypto_sha256_ctx ctx;
  crypto_sha256_init(&ctx);
  crypto_sha256_update(&ctx, data, data_len);
  crypto_sha256_final(&ctx, message_digest);
  return message_digest;
}


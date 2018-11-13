#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include <stdint.h>
#include "sha3/sha3.h"
#include "ed25519/ed25519.h"

typedef sha3_ctx_t hash_ctx_t;
#define MDSIZE  64

#define SIGNATURE_SIZE  64
#define PRIVATE_KEY_SIZE  32
#define PUBLIC_KEY_SIZE 32

typedef unsigned char byte;

extern byte sm_hash[MDSIZE];
extern byte sm_signature[SIGNATURE_SIZE];
extern byte sm_public_key[PUBLIC_KEY_SIZE];
extern byte sm_private_key[PRIVATE_KEY_SIZE];

void hash_init(hash_ctx_t* hash_ctx);
void hash_extend(hash_ctx_t* hash_ctx, const void* ptr, size_t len);
void hash_extend_page(hash_ctx_t* hash_ctx, const void* ptr);
void hash_finalize(void* md, hash_ctx_t* hash_ctx);

void sign(void* sign, const void* data, size_t len, const byte* public_key, const byte* private_key);
#endif /* crypto.h */

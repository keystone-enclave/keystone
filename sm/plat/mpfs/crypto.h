#ifndef __CRYPTO_H__
#define __CRYPTO_H__

// Include relevant libecc headers

#include <hash/sha512.h>

typedef sha512_context hash_ctx;
typedef unsigned char byte;

#define MDSIZE  64
#define SIGNATURE_SIZE  64
#define PRIVATE_KEY_SIZE  64 // includes public key
#define PUBLIC_KEY_SIZE 32

extern byte sm_hash[MDSIZE];
extern byte sm_signature[SIGNATURE_SIZE];
extern byte sm_public_key[PUBLIC_KEY_SIZE];
extern byte sm_private_key[PRIVATE_KEY_SIZE];

void hash_init(hash_ctx* hash_ctx);
void hash_extend(hash_ctx* hash_ctx, const void* ptr, size_t len);
void hash_extend_page(hash_ctx* hash_ctx, const void* ptr);
void hash_finalize(void* md, hash_ctx* hash_ctx);

void sign(void* sign, const void* data, size_t len, const byte* public_key, const byte* private_key);
int kdf(const unsigned char* salt, size_t salt_len,
        const unsigned char* ikm, size_t ikm_len,
        const unsigned char* info, size_t info_len,
        unsigned char* okm, size_t okm_len);

#endif /* __CRYPTO_H__ */

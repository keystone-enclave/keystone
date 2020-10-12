/*
 *  Copyright (C) 2020 Fraunhofer AISEC
 *  Authors: Benedikt Kopf <benedikt.kopf@aisec.fraunhofer.de>
 *           Lukas Auer <lukas.auer@aisec.fraunhofer.de>
 *           Mathias Morbitzer <mathias.morbitzer@aisec.fraunhofer.de>
 *
 *  hmac_sha3.c
 *
 *  Implements HMAC using SHA3 according to rfc2104
 *  (https://tools.ietf.org/html/rfc2104)
 *
 *  All Rights Reserved. See LICENSE for license details.
 */

#include "hmac_sha3.h"
#include <sbi/sbi_string.h>

/*
 *  Function prepare_key:
 *
 *  Description:
 *      The function prepares the given key to match the properties required
 *      for the HMAC calculation:
 *      If the given key is longer than SHA3_512_BLOCKLEN, the function hashes
 *      the key to a hash of size SHA3_512_HASH_LEN and fills the remaining
 *      bytes with 0. Otherwise the input key is copied and the remaining bytes
 *      are also filled with 0.
 *
 *  Parameters:
 *      key:        Pointer to the key
 *      key_len:    Size of the key
 *      new_key:    Pointer to the memory location, where the resulting key
 *                  should be written to !(has to be SHA3_512_BLOCK_LEN bytes
 *                  long)!
 */
static void prepare_key(const unsigned char *key, int key_len,
                        unsigned char *new_key)
{
    sha3_ctx_t ctx;

    if (key_len > SHA3_512_BLOCK_LEN) {
        sha3_init(&ctx, SHA3_512_HASH_LEN);
        sha3_update(&ctx, (void *)key, key_len);
        sha3_final(new_key, &ctx);

        key_len = SHA3_512_HASH_LEN;
    } else {
        sbi_memcpy(new_key, key, key_len);
    }

    sbi_memset(new_key + key_len, 0x00, SHA3_512_BLOCK_LEN - key_len);
}

/*
 *  Function hmac_sha3:
 *
 *  Description:
 *      Calculates the HMAC from the key and the message
 *
 *  Parameters:
 *      key:        Pointer to the key
 *      key_len:    Size of the key
 *      text:       Pointer to the message
 *      text_len:   Size of the message
 *      hmac:       Pointer to the memory location, where the result should be
 *                  written to !(size has to be SHA3_512_HASH_LEN)!
 *
 *  Return value: 0 if function has performed correctly
 */
void hmac_sha3(const unsigned char *key, int key_len,
               const unsigned char *text, int text_len, unsigned char *hmac)
{
    hmac_sha3_ctx_t ctx;

    hmac_sha3_init(&ctx, key, key_len);
    hmac_sha3_update(&ctx, text, text_len);
    hmac_sha3_final(&ctx, hmac);
}

/*
 *  Function hmac_sha3_init:
 *
 *  Description:
 *      The function initializes the hmac_sha3_ctx_t structure
 *
 *  Parameters:
 *      ctx:        Pointer to the hmac_sha3_ctx_t structure
 *      key:        Pointer to the key
 *      key_len:    Size of the key
 */
void hmac_sha3_init(hmac_sha3_ctx_t *ctx,
                    const unsigned char *key, int key_len)
{
    unsigned char temp_key[SHA3_512_BLOCK_LEN];

    prepare_key(key, key_len, ctx->key);

    // XOR with ipad
    for (int i = 0; i < SHA3_512_BLOCK_LEN; i++) {
        temp_key[i] = ctx->key[i] ^ 0x36;
    }

    sha3_init(&(ctx->sha3_ctx), SHA3_512_HASH_LEN);
    sha3_update(&(ctx->sha3_ctx), temp_key, SHA3_512_BLOCK_LEN);
}

/*
 *  Function hmac_sha3_update:
 *
 *  Description:
 *      The function updates the HMAC-SHA3 calculation with a new message
 *
 *  Parameters:
 *      ctx:        Pointer to the hmac_sha3_ctx_t structure
 *      text:       Pointer to the message
 *      text_len:   Size of the message
 */
void hmac_sha3_update(hmac_sha3_ctx_t *ctx,
                      const unsigned char *text, int text_len)
{
    if (text_len > 0)
        sha3_update(&(ctx->sha3_ctx), text, text_len);
}

/*
 *  Function hmac_sha3_final:
 *
 *  Description:
 *      The function finalizes the HMAC-SHA3 calculation and returns the final
 *      hash
 *
 *  Parameters:
 *      ctx:    Pointer to the hmac_sha3_ctx_t structure
 *      hash:   Pointer to the memory location, where the resulting hash should
 *              be written to !(has to be SHA3_512_HASH_LEN bytes long)!
 */
void hmac_sha3_final(hmac_sha3_ctx_t *ctx, unsigned char *hash)
{
    unsigned char temp_key[SHA3_512_BLOCK_LEN];
    unsigned char inner_hash[SHA3_512_HASH_LEN];

    sha3_final(inner_hash, &(ctx->sha3_ctx));

    // XOR with opad
    for (int i = 0; i < SHA3_512_BLOCK_LEN; i++) {
        temp_key[i] = ctx->key[i] ^ 0x5C;
    }

    sha3_init(&(ctx->sha3_ctx), SHA3_512_HASH_LEN);
    sha3_update(&(ctx->sha3_ctx), temp_key, SHA3_512_BLOCK_LEN);
    sha3_update(&(ctx->sha3_ctx), inner_hash, SHA3_512_HASH_LEN);
    sha3_final(hash, &(ctx->sha3_ctx));
}

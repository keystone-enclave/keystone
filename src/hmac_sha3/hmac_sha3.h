/*
 *  Copyright (C) 2020 Fraunhofer AISEC
 *  Authors: Benedikt Kopf <benedikt.kopf@aisec.fraunhofer.de>
 *           Lukas Auer <lukas.auer@aisec.fraunhofer.de>
 *           Mathias Morbitzer <mathias.morbitzer@aisec.fraunhofer.de>
 *
 *  hmac_sha3.h
 *
 *  All Rights Reserved. See LICENSE for license details.
 */

#ifndef HMAC_SHA3_H
#define HMAC_SHA3_H

#include "../sha3/sha3.h"

// Internal block length of sha3_512 in bytes
#define SHA3_512_BLOCK_LEN 72
// Output hash length of sha3_512 in bytes
#define SHA3_512_HASH_LEN 64

typedef struct {
    sha3_ctx_t sha3_ctx;
    unsigned char key[SHA3_512_BLOCK_LEN];
} hmac_sha3_ctx_t;

void hmac_sha3(const unsigned char *key, int key_len,
               const unsigned char *text, int text_len, unsigned char *hash);
void hmac_sha3_init(hmac_sha3_ctx_t *ctx,
                    const unsigned char *key, int key_len);
void hmac_sha3_update(hmac_sha3_ctx_t *ctx,
                      const unsigned char *text, int text_len);
void hmac_sha3_final(hmac_sha3_ctx_t *ctx, unsigned char *hash);

#endif /* HMAC_SHA3_H */

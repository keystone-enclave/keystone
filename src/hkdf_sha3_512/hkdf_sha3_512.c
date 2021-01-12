/*
 *  Copyright (C) 2020 Fraunhofer AISEC
 *  Authors: Benedikt Kopf <benedikt.kopf@aisec.fraunhofer.de>
 *           Lukas Auer <lukas.auer@aisec.fraunhofer.de>
 *           Mathias Morbitzer <mathias.morbitzer@aisec.fraunhofer.de>
 *
 *  hkdf_sha3_512.c
 *
 *  Implements the key derivation function according to rfc5869
 *  (https://tools.ietf.org/html/rfc5869)
 *
 *  All Rights Reserved. See LICENSE for license details.
 */

#include "hkdf_sha3_512.h"
#include "../hmac_sha3/hmac_sha3.h"
#include <sbi/sbi_string.h>

/*
 * Function div_ceil:
 *
 * Calculates ceil(op1 / op2)
 */
static int div_ceil(int op1, int op2)
{
    if (op1 % op2 == 0)
        return op1 / op2;
    else
        return (op1 / op2) + 1;
}

/*
 *  Function hkdf_sha3_512:
 *
 *  Description:
 *     Derives a key according to rfc5869 (https://tools.ietf.org/html/rfc5869)
 *
 *  Parameters:
 *     salt:           Optional salt value. Set to NULL if unused
 *     salt_len:       Size of the given salt. Set to 0 if salt is NULL
 *     ikm:            Input key
 *     ikm_length:     Size of the given input key
 *     info:           Optional context for key derivation
 *                     Set to NULL if unused
 *     info_len:       Size of the given additional information. Set to 0 if
 *                     info is NULL
 *     okm:            Pointer to the memory location, which should hold the
 *                     resulting key
 *     okm_length:     Size of the okm buffer
 *
 *  Return value: 0 if function has performed correctly
 */
int hkdf_sha3_512(const unsigned char *salt, int salt_len,
                  const unsigned char *ikm, int ikm_len,
                  const unsigned char *info, int info_len,
                  unsigned char *okm, int okm_len)
{
    unsigned char prk[SHA3_512_HASH_LEN];

    if (okm_len > 255 * SHA3_512_HASH_LEN) {
        return -1;
    }

    hkdf_extract(salt, salt_len, ikm, ikm_len, prk);

    return hkdf_expand(prk, SHA3_512_HASH_LEN, info, info_len, okm, okm_len);
}

/*
 *  Function hkdf_extract:
 *
 *  Description:
 *      Implements the extract function according to rfc5869
 *
 *  Parameters:
 *      salt:       Optional: Pointer to the buffer containing the salt. Set to
 *                  NULL if unused
 *      salt_len:   Size of the salt buffer. Set to 0 if salt is NULL
 *      ikm:        Pointer to the input key buffer
 *      ikm_len:    Size of the input key buffer
 *      prk:        Output buffer for the pseudo random key
 *                  with length SHA3_512_HASH_LEN
 */
void hkdf_extract(const unsigned char *salt, int salt_len,
                  const unsigned char *ikm, int ikm_len,
                  unsigned char *prk)
{
    unsigned char nullsalt[SHA3_512_HASH_LEN];

    if (salt == NULL || salt_len == 0) {
        sbi_memset(nullsalt, 0x00, SHA3_512_HASH_LEN);
        salt = nullsalt;
        salt_len = SHA3_512_HASH_LEN;
    }

    hmac_sha3(salt, salt_len, ikm, ikm_len, prk);
}

/*
 *  Function hkdf_expand:
 *
 *  Description:
 *      Implements the expand function according to rfc5869
 *
 *  Parameters:
 *      prk:            Pointer to the buffer containing a pseudo random key
 *      prk_len:        Size of the pseudorandom key
 *      info:           Optional: Context for key derivation. Set to NULL if
 *                      unused
 *      info_len:       Size of the given additional information. Set to 0 if
 *                      info is NULL
 *      okm:            Pointer to the memory location, which should hold the
 *                      derived key
 *      okm_length:     Size of the out_key buffer.
 *                      Must be <= 255*SHA3_512_HASH_LEN
 *
 *  Return value: 0 if function has performed correctly
 */
int hkdf_expand(const unsigned char *prk, int prk_len,
                const unsigned char *info, int info_len,
                unsigned char *okm, int okm_len)
{
    int n = div_ceil(okm_len, SHA3_512_HASH_LEN);
    unsigned char t[SHA3_512_HASH_LEN];
    hmac_sha3_ctx_t ctx;

    if (prk_len < SHA3_512_HASH_LEN) {
        return -1;
    }
    if (okm_len > 255 * SHA3_512_HASH_LEN) {
        return -1;
    }

    // Compute T(1) - T(n) and copy resulting key to okm
    for (unsigned char i = 1; i <= n; i++) {
        hmac_sha3_init(&ctx, prk, prk_len);

        if (i > 1)
            hmac_sha3_update(&ctx, t, SHA3_512_HASH_LEN);

        hmac_sha3_update(&ctx, info, info_len);
        hmac_sha3_update(&ctx, &i, 1);
        hmac_sha3_final(&ctx, t);

        if (i < n)
            sbi_memcpy(okm + (i - 1) * SHA3_512_HASH_LEN, t, SHA3_512_HASH_LEN);
        else
            sbi_memcpy(okm + (i - 1) * SHA3_512_HASH_LEN, t,
                   okm_len - (i - 1) * SHA3_512_HASH_LEN);
    }

    return 0;
}

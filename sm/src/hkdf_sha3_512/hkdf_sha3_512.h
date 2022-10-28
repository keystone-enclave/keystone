/*
 *  Copyright (C) 2020 Fraunhofer AISEC
 *  Authors: Benedikt Kopf <benedikt.kopf@aisec.fraunhofer.de>
 *           Lukas Auer <lukas.auer@aisec.fraunhofer.de>
 *           Mathias Morbitzer <mathias.morbitzer@aisec.fraunhofer.de>
 *
 *  hkdf_sha3_512.h
 *
 *  All Rights Reserved. See LICENSE for license details.
 */

#ifndef HDKF_SHA3_512_H
#define HDKF_SHA3_512_H

int hkdf_sha3_512(const unsigned char *salt, int salt_len,
                  const unsigned char *in_key, int in_key_len,
                  const unsigned char *info, int info_len,
                  unsigned char *out_key, int out_key_length);
void hkdf_extract(const unsigned char *salt, int salt_len,
                  const unsigned char *in_key, int in_key_len,
                  unsigned char *prk);
int hkdf_expand(const unsigned char *prk, int prk_len,
                const unsigned char *info, int info_len,
                unsigned char *out_key, int out_key_len);

#endif /* HDKF_SHA3_512_H */

/*
 *  Copyright (c) 2018, The Regents of the University of California (Regents).
 *
 *  Copyright (C) 2020 Fraunhofer AISEC
 *  Authors: Benedikt Kopf <benedikt.kopf@aisec.fraunhofer.de>
 *           Lukas Auer <lukas.auer@aisec.fraunhofer.de>
 *           Mathias Morbitzer <mathias.morbitzer@aisec.fraunhofer.de>
 *
 *  data-sealing.c
 *
 *  Shows how to use the sealing key feature of Keystone
 *
 *  All Rights Reserved. See LICENSE for license details.
 */

#include "app/eapp_utils.h"
#include "app/string.h"
#include "app/syscall.h"
#include "data-sealing.h"

int hextostring(const unsigned char *hex_in, size_t hex_in_size,
                char *str_out, size_t str_out_size);

/*
 *  Function main:
 *
 *  Description:
 *     Derives the sealing key
 */
int main()
{
  char *key_identifier = "identifier";
  char *key_identifier_2 = "identifier2";
  struct sealing_key key_buffer;
  int ret = 0;

  /* Derive the sealing key */
  ret = get_sealing_key(&key_buffer, sizeof(key_buffer),
                        (void *)key_identifier, strlen(key_identifier));

  size_t string_key_size = SEALING_KEY_SIZE * 2 + 1;
  char string_key[string_key_size];
  size_t string_signature_size = SIGNATURE_SIZE * 2 + 1;
  char string_signature[string_signature_size];

  hextostring((const unsigned char *)&key_buffer.key, SEALING_KEY_SIZE, string_key, string_key_size);
  hextostring((const unsigned char *)&key_buffer.signature, SIGNATURE_SIZE, string_signature, string_signature_size);

  ocall_print_buffer("Key:\n", 6);
  ocall_print_buffer(string_key, string_key_size);
  ocall_print_buffer("\nSignature:\n", 13);
  ocall_print_buffer(string_signature, string_signature_size);
  ocall_print_buffer("\n", 2);

  ret = get_sealing_key(&key_buffer, sizeof(key_buffer),
                        (void *)key_identifier_2, strlen(key_identifier_2));

  hextostring((const unsigned char *)&key_buffer.key, SEALING_KEY_SIZE, string_key, string_key_size);
  hextostring((const unsigned char *)&key_buffer.signature, SIGNATURE_SIZE, string_signature, string_signature_size);

  ocall_print_buffer("Key:\n", 6);
  ocall_print_buffer(string_key, string_key_size);
  ocall_print_buffer("\nSignature:\n", 13);
  ocall_print_buffer(string_signature, string_signature_size);
  ocall_print_buffer("\n", 2);

  if (ret) {
    ocall_print_buffer("Sealing key derivation failed!\n", 32);
    EAPP_RETURN(-1);
  } else {
    ocall_print_buffer("Sealing key derivation successful!\n", 36);
    EAPP_RETURN(0);
  }
}

/*
 *  Function ocall_print_buffer:
 *
 *  Description:
 *     Prints the buffer to the console
 */
unsigned long ocall_print_buffer(char *data, size_t data_len)
{
  unsigned long retval;

  ocall(OCALL_PRINT_BUFFER, data, data_len, &retval ,sizeof(unsigned long));

  return retval;
}

/*
 *  Function hextostring:
 *
 *  Description:
 *     Writes the string representation using the hexadecimal system into the
 *     output buffer and terminates the generated string with \0
 *
 *  Parameters:
 *     hex_in:        Pointer to the source buffer
 *     hex_in_size:   Size of the source buffer
 *     str_out:       Pointer to the buffer for the string representation
 *     str_out_size:  Size of the output buffer
 *
 *  Return value: 0 if function has performed correctly
 */
int hextostring(const unsigned char *hex_in, size_t hex_in_size,
                char *str_out, size_t str_out_size)
{
  char *hex = "0123456789ABCDEF";
  int i;

  if (str_out_size < 2 * hex_in_size + 1) {
    return -1;
  }

  for (i = 0; i < hex_in_size; i++) {
    str_out[2 * i] = hex[hex_in[i] >> 4];
    str_out[2 * i + 1] = hex[hex_in[i] & 0x0F];
  }

  str_out[2 * i] = 0x00;
  return 0;
}

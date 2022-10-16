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

/*
 *  Function main:
 *
 *  Description:
 *     Derives the sealing key
 */
int main()
{
  char *key_identifier = "identifier";
  struct sealing_key key_buffer;
  int ret = 0;

  /* Derive the sealing key */
  ret = get_sealing_key(&key_buffer, sizeof(key_buffer),
                        (void *)key_identifier, strlen(key_identifier));

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

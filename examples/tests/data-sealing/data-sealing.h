/*
 *  Copyright (C) 2020 Fraunhofer AISEC
 *  Authors: Benedikt Kopf <benedikt.kopf@aisec.fraunhofer.de>
 *           Lukas Auer <lukas.auer@aisec.fraunhofer.de>
 *           Mathias Morbitzer <mathias.morbitzer@aisec.fraunhofer.de>
 *
 *  data-sealing.h
 *
 *  All Rights Reserved. See LICENSE for license details.
 */

#include "app/sealing.h"

#define OCALL_PRINT_BUFFER 1

unsigned long ocall_print_buffer(char *data, size_t data_len);

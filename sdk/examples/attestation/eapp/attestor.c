//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "app/eapp_utils.h"
#include "app/syscall.h"
#include "edge/edge_common.h"

#define OCALL_PRINT_BUFFER 1
#define OCALL_PRINT_VALUE 2
#define OCALL_COPY_REPORT 3
#define OCALL_GET_STRING 4

int
main() {
  struct edge_data retdata;
  ocall(OCALL_GET_STRING, NULL, 0, &retdata, sizeof(struct edge_data));

  for (unsigned long i = 1; i <= 10000; i++) {
    if (i % 5000 == 0) {
      ocall(OCALL_PRINT_VALUE, &i, sizeof(unsigned long), 0, 0);
    }
  }

  char nonce[2048];
  if (retdata.size > 2048) retdata.size = 2048;
  copy_from_shared(nonce, retdata.offset, retdata.size);

  char buffer[2048];
  attest_enclave((void*)buffer, nonce, retdata.size);

  ocall(OCALL_COPY_REPORT, buffer, 2048, 0, 0);

  EAPP_RETURN(0);
}

//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "eapp_utils.h"
#include "string.h"
#include "edge_call.h"
#include <syscall.h>

#define OCALL_PRINT_BUFFER 1

unsigned long ocall_print_buffer(char* data, unsigned long data_len);

int main(){

  /* Put your ocall here */

  EAPP_RETURN(0);
}

unsigned long ocall_print_buffer(char* data, unsigned long data_len){
  unsigned long retval;
  ocall(OCALL_PRINT_BUFFER, data, data_len, &retval ,sizeof(unsigned long));
  return retval;
}

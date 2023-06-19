//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "edge/edge_call.h"
#include "edge_wrapper.h"
#include "app/eapp_utils.h"
#include "app/string.h"
#include "app/syscall.h"

void edge_init(){
  /* Nothing for now, will probably register buffers/callsites
     later */
}

void ocall_print_value(unsigned long val){

  unsigned long val_ = val;
  ocall(2, &val_, sizeof(unsigned long), 0, 0);

  return;
}

unsigned long ocall_print_buffer(char* data, size_t data_len){

  unsigned long retval;
  ocall(1, data, data_len, &retval ,sizeof(unsigned long));

  return retval;
}

void ocall_copy_report(void* report, size_t len) {
  ocall(3, report, len, 0, 0);
}

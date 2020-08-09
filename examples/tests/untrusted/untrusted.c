//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "app/eapp_utils.h"
#include "app/string.h"
#include "app/syscall.h"
#include "app/malloc.h"
#include "edge_wrapper.h"

void EAPP_ENTRY eapp_entry(){

  char* msg = "hello world!\n";
  char* msg2 = "2nd hello world!\n";

  edge_init();

  unsigned long ret = ocall_print_buffer(msg, 13);
  ocall_print_buffer(msg2, 17);

  ocall_print_value(ret);

  struct edge_data pkgstr;
  ocall_get_string(&pkgstr);

  void* host_str = malloc(pkgstr.size);
  copy_from_shared(host_str, pkgstr.offset, pkgstr.size);

  int i;
  int ct;
  for(i = 0; i < pkgstr.size; i++){
    if( ((char*)host_str)[i] == 'l' ){
      ct++;
    }
  }

  ocall_print_value(ct);

  EAPP_RETURN(ret);
}

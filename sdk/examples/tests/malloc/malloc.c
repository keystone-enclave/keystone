//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "malloc.h"

#include "app/eapp_utils.h"

void EAPP_ENTRY
eapp_entry() {
  int arg;
  int* ptr = (int*)malloc(sizeof(int));
  *ptr     = 11411;
  arg      = *ptr;

  EAPP_RETURN(arg);
}

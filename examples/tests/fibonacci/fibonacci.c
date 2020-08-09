//******************************************************************************
// Copyright (c) 2018, The Regents of the University of California (Regents).
// All Rights Reserved. See LICENSE for license details.
//------------------------------------------------------------------------------
#include "app/eapp_utils.h"

unsigned long fibonacci_rec(unsigned long in){
  if( in <= 1)
    return 1;
  else
    return fibonacci_rec(in-1)+fibonacci_rec(in-2);
}

void EAPP_ENTRY eapp_entry(){
  int arg = 35;
  EAPP_RETURN(fibonacci_rec(arg));
}

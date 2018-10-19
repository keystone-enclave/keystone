#include "eapp_utils.h"

unsigned long fibonacci_rec(unsigned long in){
  if( in <= 1)
    return 1;
  else
    return fibonacci_rec(in-1)+fibonacci_rec(in-2);
}

void eapp_entry(){
  EAPP_ARGUMENT(arg);
  arg = 35;
  EAPP_RETURN(fibonacci_rec(arg));
}

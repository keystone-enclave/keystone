#include "eapp_utils.h"
#include "malloc.h"

void eapp_entry(){
  EAPP_ARGUMENT(arg);
  
  int* ptr = (int*) malloc(sizeof(int));
  *ptr = 11411;
  arg = *ptr;

  EAPP_RETURN(arg);
}

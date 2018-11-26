#include "eapp_utils.h"
#include "malloc.h"

void EAPP_ENTRY eapp_entry(){
  int arg;
  int* ptr = (int*) malloc(sizeof(int));
  *ptr = 11411;
  arg = *ptr;

  EAPP_RETURN(arg);
}

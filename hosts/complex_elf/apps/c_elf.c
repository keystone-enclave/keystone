#include "eapp_utils.h"

unsigned int arr_data[] = {0,2,4,6,8,10,12};

unsigned int arr_lookup(int v){
  return arr_data[v];
}

void eapp_entry(){
  EAPP_ARGUMENT(arg);

  EAPP_RETURN(arr_lookup(arg)*10);
}

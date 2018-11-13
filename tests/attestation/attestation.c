#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"

#include "edge_wrapper.h"

void EAPP_ENTRY eapp_entry(){
  edge_init();

  char* data = "nonce";
  char buffer[2048];

  attest_enclave((void*) buffer, data, 5); 

  ocall_copy_report(buffer, 2048);
  
  EAPP_RETURN(0);
}

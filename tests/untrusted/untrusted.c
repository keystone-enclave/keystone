#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"
#include "malloc.h"
#include "edge_wrapper.h"

void EAPP_ENTRY eapp_entry(){

  char* msg = "hello world!\n";
  char* msg2 = "2nd hello world!\n";
  
  edge_init();
  
  unsigned long ret = ocall_print_buffer(msg, 13);
  ocall_print_buffer(msg2, 17);

  ocall_print_value(ret);

  packaged_str_t pkgstr = ocall_get_string();

  
  void* host_str = malloc(pkgstr.len);
  copy_from_shared(host_str, pkgstr.str_offset, pkgstr.len);

  int i;
  int ct;
  for(i = 0; i < pkgstr.len; i++){
    if( ((char*)host_str)[i] == 'l' ){
      ct++;
    }
  }

  ocall_print_value(ct);
  
  EAPP_RETURN(ret);
}

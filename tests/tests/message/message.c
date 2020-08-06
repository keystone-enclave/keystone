#include "eapp_utils.h"
#include "syscall.h"

void EAPP_ENTRY eapp_entry(){
   char* msg = "hello world!\n";
   send_msg(1, msg, 13);  
}

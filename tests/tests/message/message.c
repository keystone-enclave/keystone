#include "eapp_utils.h"
#include "syscall.h"
#include "edge_wrapper.h"

void EAPP_ENTRY eapp_entry(){
   char* msg1 = "MESSAGE 1!\n";
   char *msg2 = "MESSAGE 2!\n"; 
   char *msg3 = "MESSAGE 3!\n"; 

   char buf[13]; 
   int uid = 0; 
   edge_init(); 

   send_msg(uid, msg1, 13);
   send_msg(uid, msg2, 13);
   send_msg(uid, msg3, 13);

   recv_msg(uid, buf, 13);
   ocall_print_buffer(buf, 13);
   
   recv_msg(uid, buf, 13);
   ocall_print_buffer(buf, 13);

   recv_msg(uid, buf, 13);
   ocall_print_buffer(buf, 13);

   EAPP_RETURN(0); 
}

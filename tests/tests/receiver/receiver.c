#include "eapp_utils.h"
#include "syscall.h"
#include "edge_wrapper.h"
#include <string.h> 

#define BUF_SIZE 256

void EAPP_ENTRY eapp_entry(){
   char buf[BUF_SIZE]; 
   size_t uid;
   get_uid(&uid); 

   edge_init(); 

   while(recv_msg(uid - 1, buf, BUF_SIZE));
   ocall_print_buffer(buf, BUF_SIZE);
  
   while(recv_msg(uid - 1, buf, BUF_SIZE));
   ocall_print_buffer(buf, BUF_SIZE);

   while(recv_msg(uid - 1, buf, BUF_SIZE));
   ocall_print_buffer(buf, BUF_SIZE); 

   while(send_msg(uid - 1, "DONE!\n", strlen("DONE!\n") + 1));

   EAPP_RETURN(0); 
}

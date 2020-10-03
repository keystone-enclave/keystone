#include "eapp_utils.h"
#include "syscall.h"
#include "edge_wrapper.h"
#include <string.h> 

#define BUF_SIZE 256

void EAPP_ENTRY eapp_entry(){
   char* msg1 = "MESSAGE 1!\n";
   char *msg2 = "MESSAGE 2!\n"; 
   char *msg3 = "MESSAGE 3!\n"; 

   char *msg4 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n"; 
   char *error1 = "Oversized message detected successfully!\n";
   char *error2 = "Empty mailbox detected successfully!\n";

   char buf[256];
   size_t uid; 
   unsigned long ret;

   get_uid(&uid); 
   edge_init(); 

   while(send_msg(uid + 1, msg1, strlen(msg1) + 1));
   while(send_msg(uid + 1, msg2, strlen(msg2) + 1));
   while(send_msg(uid + 1, msg3, strlen(msg3) + 1));

   while(recv_msg(uid + 1, buf, BUF_SIZE)); 
   ocall_print_buffer(buf,  strlen("DONE!\n") + 1);
   
   //This should fail because the message is too big.  
   ret =  send_msg(uid + 1, msg4, strlen(msg4) + 1); 
   if(ret){
      ocall_print_buffer(error1, strlen(error1) + 1); 
   }

   //This should fail because no message is in the mailbox
   ret = recv_msg(uid + 1, buf, BUF_SIZE);
   if(ret){
      ocall_print_buffer(error2,  strlen(error2) + 1); 
   }

   EAPP_RETURN(0); 
}

#include "eapp_utils.h"
#include "syscall.h"
#include "edge_wrapper.h"
#include <string.h> 

#define BUF_SIZE 13

void EAPP_ENTRY eapp_entry(){
   char* msg1 = "MESSAGE 1!\n";
   char *msg2 = "MESSAGE 2!\n"; 
   char *msg3 = "MESSAGE 3!\n"; 
   char *msg4 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.\n"; 

   char buf[BUF_SIZE]; 
   int uid = get_uid(); 
   unsigned long ret; 

   edge_init(); 

   send_msg(uid, msg1, strlen(msg1) + 1);
   send_msg(uid, msg2, strlen(msg2) + 1);
   send_msg(uid, msg3, strlen(msg3) + 1);
   
   //This should fail because the message is too big.  
   ret =  send_msg(uid, msg4, strlen(msg4) + 1); 

   recv_msg(uid, buf, BUF_SIZE);
   ocall_print_buffer(buf, BUF_SIZE);
   
   recv_msg(uid, buf, BUF_SIZE);
   ocall_print_buffer(buf, BUF_SIZE);

   recv_msg(uid, buf, BUF_SIZE);
   ocall_print_buffer(buf, BUF_SIZE);

   //This should print out 1
   ocall_print_value(ret); 

   //This should fail because on message is in the mailbox
   ret = recv_msg(uid, buf, BUF_SIZE);
   ocall_print_value(ret);   

   EAPP_RETURN(0); 
}

#include "eapp_utils.h"
#include "syscall.h"
#include "edge_wrapper.h"
#include <string.h> 

#define BUF_SIZE 256
#define EYRIE_LOAD_START        0xffffffff00000000

struct mem_share_req{
     uintptr_t base_addr; 
     size_t enclave_size; 
     size_t offset;
};

void EAPP_ENTRY eapp_entry(){
   char buf[BUF_SIZE]; 
   size_t uid;
   struct mem_share_req req; 

   get_uid(&uid); 

   char *ack_msg = "ACK!\n"; 
   char *done_msg = "DONE!\n";
   uintptr_t ptr =  0x700000000; 
   size_t *target_ptr;

   edge_init(); 

   //Listen for MEMSHARE START request from granter
   while(recv_msg(uid - 1, buf, BUF_SIZE));

   ocall_print_buffer("[GRANTEE] RECEIVED MEM_SHARE REQUEST\n",  strlen("[GRANTEE] RECEIVED MEM_SHARE REQUEST\n") + 1);

   //Sends ACK to granter 
   while(send_msg(uid - 1, ack_msg, strlen(ack_msg) + 1)); 
  
   //Reads request from client 
   while(recv_msg(uid - 1, &req, sizeof(struct mem_share_req))); 

   //mmap req.base_addr 
   ptr = (uintptr_t) map(req.base_addr, req.enclave_size, ptr); 
   
   target_ptr = (size_t *) (ptr + req.offset); 
   *target_ptr = 1337; 

   mem_stop(uid - 1); 

   while(send_msg(uid - 1, done_msg, strlen(done_msg) + 1));

   EAPP_RETURN(0); 
}

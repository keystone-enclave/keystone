#include "eapp_utils.h"
#include "syscall.h"
#include "edge_wrapper.h"
#include <string.h> 

#define BUF_SIZE 256

struct mem_share_req{
    uintptr_t base_addr;
    size_t enclave_size;
    uintptr_t offset;  
}; 

void EAPP_ENTRY eapp_entry(){
   char buf[BUF_SIZE]; 
   size_t uid;
   size_t dummy_value = 5; 

   uintptr_t base_addr;
   size_t enclave_size; 

   struct mem_share_req req;
   req.offset = translate((uintptr_t) &dummy_value);     
 
   get_uid(&uid); 

   char *start_msg = "START MEMSHARE\n";

   edge_init(); 

   //Send start MEMSHARE to grantee
   while(send_msg(uid + 1, start_msg, strlen(start_msg) + 1));

   ocall_print_buffer("[GRANTER] MEMSHARE START\n",  strlen("[GRANTER] MEMSHARE START\n") + 1);

   //Receive ACK from grantee
   while(recv_msg(uid + 1, buf, BUF_SIZE));

   ocall_print_buffer("[GRANTER] ACK RECEIVED\n", strlen("[GRANTER] ACK RECEIVED\n"));

   //Begin MEMSHARE process
   mem_share(uid + 1, &base_addr, &enclave_size);

   req.base_addr = base_addr; 
   req.enclave_size = enclave_size;
   req.offset = req.offset - base_addr;  

   while(send_msg(uid + 1, &req, sizeof(struct mem_share_req)));

   //Busy wait until grantee is finished
   while(recv_msg(uid + 1, buf, BUF_SIZE));

   ocall_print_value(dummy_value);

   EAPP_RETURN(0); 
}

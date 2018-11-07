#include <edge_call.h>
#include "edge_wrapper.h"
#include "eapp_utils.h"
#include "string.h"
#include "syscall.h"


uintptr_t shared_buffer;
size_t shared_buffer_size = 0x1000;
uintptr_t buffer_data_start;

void edge_init(){
  shared_buffer = untrusted_mmap();
  buffer_data_start = shared_buffer + sizeof(struct edge_call_t);
}

/* TODO eventually this interface will work differently, and be
 * handled by the runtime primarily. For now the userspace does the
 * copy */
unsigned long ocall_print_buffer(char* data, size_t data_len){
  /* For now we assume by convention that the start of the buffer is
   * the right place to put calls */
  struct edge_call_t* edge_call = (struct edge_call_t*)shared_buffer;
  edge_call->call_id = 1;
  memcpy((void*)buffer_data_start, (void*)data, data_len);

  if(edge_call_get_offset_from_ptr(shared_buffer, shared_buffer_size,
				   buffer_data_start, data_len,
				   &(edge_call->call_arg_offset)) != 0){
    // TODO another failure case
    return 10+edge_call_get_offset_from_ptr(shared_buffer, shared_buffer_size,
					    buffer_data_start, data_len,
					    &(edge_call->call_arg_offset));
  }
  /* TODO: change this to be just a notify of the runtime that ocall
   * is ready */
  int ret = ocall(1);

  if (ret != 0) {
    return 10;
    // TODO another failure case
  }

  if(edge_call->return_data.call_status != CALL_STATUS_OK){
    // TODO what to do with status code?
    return edge_call->return_data.call_status;
  }


  uintptr_t return_ptr;
  if(edge_call_get_ptr_from_offset(shared_buffer, shared_buffer_size,
				   edge_call->return_data.call_ret_offset, sizeof(unsigned long),
				   &return_ptr) != 0){
    // TODO another failure case
    return 11;
  }

  return *(unsigned long*)return_ptr;
}

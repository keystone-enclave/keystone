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
void* ocall_dispatch( unsigned long call_id, void* data, size_t data_len, size_t return_len){
  /* For now we assume by convention that the start of the buffer is
   * the right place to put calls */

  struct edge_call_t* edge_call = (struct edge_call_t*)shared_buffer;
  edge_call->call_id = call_id;
  memcpy((void*)buffer_data_start, (void*)data, data_len);

  if(edge_call_get_offset_from_ptr(shared_buffer, shared_buffer_size,
				   buffer_data_start, data_len,
				   &(edge_call->call_arg_offset)) != 0){
    /* TODO In the future, this should fault */
    return NULL;
  }

  /* TODO this shouldn't need a call_id */
  int ret = ocall(1);

  if (ret != 0) {
    /* TODO In the future, this should fault */
    return NULL;
  }

  if(edge_call->return_data.call_status != CALL_STATUS_OK){
    /* TODO In the future, this should fault */
    return &edge_call->return_data.call_status;
  }

  if( return_len == 0 ){
    /* Done, no return */
    return NULL;
  }

  uintptr_t return_ptr;
  if(edge_call_get_ptr_from_offset(shared_buffer, shared_buffer_size,
				   edge_call->return_data.call_ret_offset, return_len,
				   &return_ptr) != 0){
    /* TODO In the future, this should fault */
    return NULL;
  }

  return (void*)return_ptr;

  
}


void ocall_print_value(unsigned long val){

  unsigned long val_ = val;
  /* TODO This will be an ocall syscall at some point */
  ocall_dispatch(2, &val_, sizeof(unsigned long), 0);

  return;
}

unsigned long ocall_print_buffer(char* data, size_t data_len){

  /* TODO This will be an ocall syscall at some point */
  void* return_ptr = ocall_dispatch(1, data, data_len, sizeof(unsigned long));

  return *(unsigned long*)return_ptr;
}

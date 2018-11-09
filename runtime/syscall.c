#include <stdint.h>
#include <stddef.h>
#include "syscall.h"
#include "string.h"
#include <edge_call.h>
#include "uaccess.h"

extern void exit_enclave(uintptr_t arg0);

/* TODO Get these from SM/host */
#define shared_buffer (uintptr_t)0x0dead000
#define shared_buffer_size (size_t)0x1000
#define buffer_data_start (uintptr_t)(shared_buffer + sizeof(struct edge_call_t))

uintptr_t dispatch_edgecall_ocall( unsigned long call_id,
				   void* data, size_t data_len,
				   void* return_buffer, size_t return_len){

  uintptr_t ret;
  /* For now we assume by convention that the start of the buffer is
   * the right place to put calls */

  struct edge_call_t* edge_call = (struct edge_call_t*)shared_buffer;
  edge_call->call_id = call_id;
  copy_from_user((void*)buffer_data_start, (void*)data, data_len);

  if(edge_call_get_offset_from_ptr(shared_buffer, shared_buffer_size,
				   buffer_data_start, data_len,
				   &(edge_call->call_arg_offset)) != 0){
    goto ocall_error;
  }

  ret = SBI_CALL_1(SBI_SM_STOP_ENCLAVE, 1);

  if (ret != 0) {
    goto ocall_error;
  }

  if(edge_call->return_data.call_status != CALL_STATUS_OK){
    goto ocall_error;
  }

  if( return_len == 0 ){
    /* Done, no return */
    return (uintptr_t)NULL;
  }

  uintptr_t return_ptr;
  if(edge_call_get_ptr_from_offset(shared_buffer, shared_buffer_size,
				   edge_call->return_data.call_ret_offset, return_len,
				   &return_ptr) != 0){
    goto ocall_error;
  }

  /* Done, there was a return value to copy out of shared mem */
  copy_to_user(return_buffer, (void*)return_ptr, return_len);
  
  return 0;

 ocall_error:
  /* TODO In the future, this should fault */
  return 1;
}

void handle_syscall(struct encl_ctx_t* ctx)
{
  
  uintptr_t n = ctx->regs.a7;
  uintptr_t arg0 = ctx->regs.a0;
  uintptr_t arg1 = ctx->regs.a1;
  uintptr_t arg2 = ctx->regs.a2;
  uintptr_t arg3 = ctx->regs.a3;
  uintptr_t arg4 = ctx->regs.a4;

  uintptr_t ret = 0;

  ctx->regs.sepc += 4;

  //printf("[runtime] syscall: %ld\n", n);
  switch (n) {
    case(RUNTIME_SYSCALL_EXIT):
      SBI_CALL_1(SBI_SM_EXIT_ENCLAVE, arg0);
      break;
    case(RUNTIME_SYSCALL_OCALL):
      ret = dispatch_edgecall_ocall(arg0, (void*)arg1, arg2, (void*)arg3, arg4);
      //      ret = SBI_CALL_1(SBI_SM_STOP_ENCLAVE, arg0);
      break;
    /* FIXME: just fixed mapping now */
    case(RUNTIME_SYSCALL_UNTRUSTED_MMAP):
      ret = 0x0dead000;
      break;
    case(RUNTIME_SYSCALL_UNKNOWN):
    default:
      printf("[runtime] syscall %ld not implemented\n", (unsigned long) n);
      ret = -1;
      break;
  }

  /* store the result in the stack */
  ctx->regs.a0 = ret;
  return;
}

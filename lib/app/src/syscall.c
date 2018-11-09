#include "syscall.h"

/* this implementes basic system calls for the enclave */

int ocall(unsigned long call_id,
	  void* data, size_t data_len,
	  void* return_buffer, size_t return_len){
  return SYSCALL_5(SYSCALL_OCALL, call_id, data, data_len, return_buffer, return_len);
}

/* FIXME: this should not be provided to the user */
uintptr_t untrusted_mmap()
{
  return SYSCALL_0(SYSCALL_UNTRUSTED_MMAP);
}

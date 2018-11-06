#include "syscall.h"

/* this implementes basic system calls for the enclave */

uintptr_t ocall(uintptr_t request)
{
  return SYSCALL_1(SYSCALL_OCALL, request);
}

/* FIXME: this should not be provided to the user */
uintptr_t untrusted_mmap()
{
  return SYSCALL_0(SYSCALL_UNTRUSTED_MMAP);
}

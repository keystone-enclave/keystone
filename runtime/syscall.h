#ifndef __SYSCALL_H__
#define __SYSCALL_H__

#include "printf.h"
#include "regs.h"

#define RUNTIME_SYSCALL_UNKNOWN         1000
#define RUNTIME_SYSCALL_OCALL           1001
#define RUNTIME_SYSCALL_SHAREDCOPY      1002
#define RUNTIME_SYSCALL_ATTEST_ENCLAVE  1003
#define RUNTIME_SYSCALL_EXIT            1101

void handle_syscall(struct encl_ctx_t* ctx);

#endif /* syscall.h */

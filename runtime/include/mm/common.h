#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>
#include "call/sbi.h"
#include "util/printf.h"

#define RISCV_EXCP_INST_MISALIGNED  0
#define RISCV_EXCP_INST_FAULT       1
#define RISCV_EXCP_ILLEGAL_INST     2
#define RISCV_EXCP_BREAKPOINT       3
#define RISCV_EXCP_LOAD_MISALIGNED  4
#define RISCV_EXCP_LOAD_FAULT       5
#define RISCV_EXCP_STORE_MISALIGNED 6
#define RISCV_EXCP_STORE_FAULT      7
#define RISCV_EXCP_ECALL_U          8
#define RISCV_EXCP_ECALL_S          9
// reserved                         10
#define RISCV_EXCP_ECALL_M          11
#define RISCV_EXCP_INST_PAGE_FAULT  12
#define RISCV_EXCP_LOAD_PAGE_FAULT  13
// reserved                         14
#define RISCV_EXCP_STORE_PAGE_FAULT 15

#undef assert
#define assert(x) \
  if(!(x)) { printf("assertion failed at %s:%d\r\n", __FILE__, __LINE__);\
    sbi_exit_enclave(-1); \
  }

#ifdef USE_DEBUG
#define debug(format, ...) \
  printf ("[debug] " format " (%s:%d)\r\n", ## __VA_ARGS__, __FILE__, __LINE__)
#else
#define debug(format, ...) \
  ;
#endif

#define warn(format, ...) \
  printf ("[warn] " format " (%s:%d)\r\n", ## __VA_ARGS__, __FILE__, __LINE__)

#endif

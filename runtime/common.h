#ifndef __COMMON_H__
#define __COMMON_H__

#include "sbi.h"
#include "printf.h"

#define assert(x) \
  if(!(x)) { printf("assertion failed at %s:%d\n", __FILE__, __LINE__);\
    sbi_exit_enclave(-1); \
  }

#endif

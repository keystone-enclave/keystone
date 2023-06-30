#ifndef __SM_ASSERT_H__

#include <sbi/sbi_console.h>
#include <sbi/sbi_hart.h>

#define sm_assert(cond) { \
  if (!(cond)) { \
    sbi_printf("[SM] assertion_failed\r\n"); \
    sbi_hart_hang(); \
  } \
}

#endif

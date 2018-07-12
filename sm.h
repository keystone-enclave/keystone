#ifndef sm_h
#define sm_h

//FIXME: just arbitrary 128MB range
#include <stdint.h>
#include "pmp.h"
#include "sm-sbi.h"
#include "encoding.h"

#define SMM_BASE  0x80000000
#define SMM_SIZE  0x200000

void sm_init(void);

#endif

#include "mm.h"

static struct enclave_mm mm;

void init_mm(unsigned long stack_size)
{
  mm.stack_ptr = rt_base;
  mm.stack_size = stack_size; 
}

#ifndef __MM_H__
#define __MM_H__i

extern unsigned long rt_base;

struct enclave_mm {
  unsigned long stack_ptr;
  unsigned long stack_size; 
};

void init_mm(unsigned long stack_size);

#endif

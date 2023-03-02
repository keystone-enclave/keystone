
#include <stdio.h>

#define MEMORY_DEFINES_ONLY
#include "host/Memory.hpp"

__attribute__((section(".keystone")))
__attribute__((aligned(RISCV_PGSIZE)))
__attribute__((used))
int test1(int initial)
{
  int i = initial;
  char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

  while(1) {
    printf("%c", alphabet[i]);
    i = (i + 1) % sizeof(alphabet);
  }
}
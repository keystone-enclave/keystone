#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "apps/c_elf_entry.h"

#define SECRET_DATA "hello, world!\n"
#define SECRET_SIZE 15

int main()
{

  Keystone enclave;
  keystone_status_t err;
  char buf[SECRET_SIZE] = {0,};

  enclave.init_elf("c_elf.eapp_riscv", 4097, c_elf_entry);
  enclave.run();

  return 0;
}


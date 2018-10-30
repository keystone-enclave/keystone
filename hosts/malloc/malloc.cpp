#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "apps/malloc_entry.h"

int main()
{

  Keystone enclave;
  keystone_status_t err;

  enclave.init_elf("malloc.eapp_riscv", 4096, malloc_entry);
  enclave.run();

  return 0;
}


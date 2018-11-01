#include <iostream>
#include <cstdio>
#include "keystone.h"

int main(int argc, char** argv)
{
  if(argc != 2)
  {
    printf("Usage: %s <eapp>\n");
    return 0;
  }
  Keystone enclave;
  keystone_status_t err;

  enclave.init_elf((char*)argv[1], 4097, 0x1000);
  enclave.run();

  return 0;
}


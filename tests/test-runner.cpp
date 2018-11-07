#include <iostream>
#include <cstdio>
#include "keystone.h"

void print_buffer(Keystone* enclave)
{
  printf("%s\n", enclave->getBuffer());
}

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    printf("Usage: %s <eapp> <runtime>\n", argv[0]);
    return 0;
  }
  Keystone enclave;
  keystone_status_t err;
  uintptr_t retval;

  enclave.registerOcall(1, print_buffer);
  enclave.init(argv[1], argv[2], 4096, 4096, 0x1000);
  enclave.run();


  return 0;
}


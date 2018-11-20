#include <iostream>
#include <cstdio>
#include "keystone.h"

int main(int argc, char** argv)
{
  if(argc != 3)
  {
    printf("Usage: %s <eapp> <runtime>\n", argv[0]);
  return 0;
  }
  Keystone enclave;
  Params params;

  params.setEnclaveEntry(0x1000);
  params.setUntrustedMem(0xffffffffdead0000, 8192);

  enclave.init(argv[1], argv[2], params);
  enclave.run();

  return 0;
}


#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "edge_wrapper.h"


unsigned long print_buffer(char* str){
  printf("Enclave said: %s",str);
  return strlen(str);
}

void print_value(unsigned long val){
  printf("Enclave said value: %u\n",val);
  return;
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

  enclave.init(argv[1], argv[2], 4096, 4096, 0x1000);

  edge_init(&enclave);

  enclave.run();

  return 0;
}


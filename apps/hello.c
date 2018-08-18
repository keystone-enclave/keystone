#include <iostream>
#include <cstdio>
#include "keystone.h"
#include "../apps/simple_func/add.h"

#define SECRET_DATA "hello, world!\n"
#define SECRET_SIZE 15

int main()
{

  Keystone enclave;
  keystone_status_t err;
  char buf[SECRET_SIZE] = {0,};

  enclave.init();
  //enclave.copyToEnclave((void*) add_o, add_o_len);
  enclave.run((void*)0xffffffffc0000000);
  
  return 0;
}


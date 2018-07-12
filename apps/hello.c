#include <iostream>
#include "keystone.h"

#define SECRET_DATA "hello, world!\n"
#define SECRET_SIZE 15
int main()
{

  Keystone enclave;
  keystone_status_t err;
  char buf[SECRET_SIZE] = {0,};

  err = enclave.init();
  if(err) {
    std::cout<<"failed to initialize the enclave"<<std::endl;
    exit(err);
  }

  err = enclave.copy_to_encl((void*)SECRET_DATA, SECRET_SIZE);
  if(err) {
    std::cout<<"failed to copy data into the enclave"<<std::endl;
    exit(err);
  }

  err = enclave.copy_from_encl(buf, SECRET_SIZE);
  if(err) {
    std::cout<<"failed to copy data from the enclave"<<std::endl;
    exit(err);
  }

  std::cout<<buf<<std::endl;

  return 0;
}

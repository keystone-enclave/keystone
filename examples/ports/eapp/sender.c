
#include <stdlib.h>

#include "app/syscall.h"
#include "app/callee.h"

int main() {
  int i, ret;

  char alphabet[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIKLMNOPQRSTUVWXYZ0123456789";

  while(1) {
    i = rand() % sizeof(alphabet);

    ret = call_enclave(0, CALL_RECEIVER, 1, alphabet[i]);
    if(ret < 0)
      break;
  }

  call_enclave(0, CALL_RECEIVER, 1, 0xff);
}
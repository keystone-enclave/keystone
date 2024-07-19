#include <stdio.h>
#include <unistd.h>

int main()
{
  printf("Hello, Enclave!\n");
  sleep(30);
  printf("Goodbye, Enclave!\n");
  return 0;
}

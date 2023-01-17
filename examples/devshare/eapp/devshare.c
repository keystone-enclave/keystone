#include <stdio.h>
#include <string.h>

#include "app/syscall.h"

#define SECURE_DEVICE  "uart@10001000"

int main()
{
  int ret, i;
  ret = claim_mmio(SECURE_DEVICE,
                   strlen(SECURE_DEVICE));
  if(ret < 0) {
    printf("Failed to claim " SECURE_DEVICE "\n");
    return -1;
  }

  for(i = 0; i < 1000; i++) {
    printf("Writing to UART: %i!\n", i);
    fflush(stdout);
  }

  // todo do something with the device

  ret = release_mmio(SECURE_DEVICE,
                     strlen(SECURE_DEVICE));
  if(ret < 0) {
    printf("Failed to release " SECURE_DEVICE "\n");
  }

  return 0;
}

#include <stdio.h>
#include <string.h>

#include "app/syscall.h"

#define SECURE_DEVICE  "uart@10001000"

int main()
{
  int ret, fd, i;
  ret = claim_mmio(SECURE_DEVICE,
                   strlen(SECURE_DEVICE));
  if(ret < 0) {
    printf("Failed to claim " SECURE_DEVICE "\n");
    return -1;
  }

  fd = openat(-2, "uart8250", 0, 0);
  if(fd < 0) {
    printf("Failed to get fd for device\n");
    return -1;
  }

  for(i = 0; i < 1000; i++) {
    fprintf(fd, "Writing to UART: %i!\n", i);
    fflush(fd);
  }

  // todo do something with the device

  ret = release_mmio(SECURE_DEVICE,
                     strlen(SECURE_DEVICE));
  if(ret < 0) {
    printf("Failed to release " SECURE_DEVICE "\n");
  }

  return 0;
}

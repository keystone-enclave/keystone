#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <asm-generic/ioctl.h>

#include "../../../include/host/common.h"

#include "../../../include/host/keystone_user.h"

int
main(int argc, char** argv) {
  int fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  if (ioctl(fd, KEYSTONE_IOC_START_MGMT_CORE) == -1) {
    printf("Management core failed to start! errno: %d\n");
    while (1);
  }
  printf("Management core started.\n");
  return 0;
}
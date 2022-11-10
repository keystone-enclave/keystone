#include <stdio.h>
#include <fcntl.h>

// #include "../../../include/host/common.h"
#define KEYSTONE_DEV_PATH "/dev/keystone_enclave"

// #include "../../../include/host/keystone_user.h"
#include "/usr/include/asm-generic/ioctl.h"
#define KEYSTONE_IOC_MAGIC 0xa4
#define KEYSTONE_IOC_START_MGMT_CORE _IO(KEYSTONE_IOC_MAGIC, 0x08)

int
main(int argc, char** argv) {
  int fd = open(KEYSTONE_DEV_PATH, O_RDWR);
  ioctl(fd, KEYSTONE_IOC_START_MGMT_CORE);
  return 0;
}
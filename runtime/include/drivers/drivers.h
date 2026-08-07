#ifndef _DRIVERS_H_
#define _DRIVERS_H_

#include <stddef.h>
#include <stdint.h>
#include <sys/types.h>

#define DRIVER_NAME_MAX_LEN 32

typedef struct _driver {
  char name[DRIVER_NAME_MAX_LEN];
  int (*init)(void);
  int (*fini)(void);
  size_t (*read)(void*, size_t);
  size_t (*write)(void*, size_t);
} driver;

/**
 * @brief
 * Use the "driver_instance" macro to simultaneously define a new
 * driver struct and also add it to the eyrie-rt binary.
 */

#define driver_instance __attribute__((section(".drivers"))) driver

int init_driver_subsystem();

int init_driver(char *name);

int fini_driver(char *name);

uintptr_t drivers_openat_override(int dirfd, char* path, int flags, mode_t mode);

uintptr_t drivers_close_override(int fd);

uintptr_t drivers_read_override(int fd, void* buf, size_t len);

uintptr_t drivers_write_override(int fd, void* buf, size_t len);

#endif  // _DRIVERS_H_

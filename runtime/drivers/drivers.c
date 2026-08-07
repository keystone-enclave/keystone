#include "drivers/drivers.h"

#include <stdint.h>
#include <sys/param.h>
#include <sys/types.h>

#include "call/io_wrap.h"
#include "call/syscall.h"
#include "uaccess.h"
#include "util/string.h"

#define FD_COUNT 64
#define EYRIE_FD_OFFSET 10000

#define EYRIE_DEVICES_DIRFD -2

driver* drivers_head;
driver* drivers_tail;
uint32_t drivers_len;

driver* fd_nodes[FD_COUNT];

#ifdef USE_DRIVERS
extern uintptr_t drivers_start; // defined in runtime.ld.S
extern uintptr_t drivers_end; // defined in runtime.ld.S

int init_driver_subsystem() {
  for (int i = 0; i < FD_COUNT; i += 1)
    fd_nodes[i] = NULL;

  drivers_head = (driver*)&drivers_start;
  drivers_tail = (driver*)&drivers_end;
  drivers_len = drivers_tail - drivers_head;
  return 1;
}
#endif

driver* get_driver_from_name(char* name) {
  // Get a local copy of the name so we can change it if we need
  int i;
  char name_local[DRIVER_NAME_MAX_LEN] = {0};

  size_t name_len = MIN(strlen(name), DRIVER_NAME_MAX_LEN);
  memcpy(name_local, name, name_len);

  // Check if we were passed a devicetree-style name, and fix it if so
  for(i = 0; i < name_len; i++) {
    if(name_local[i] == '@') {
      name_local[i] = '\0';
      break;
    }
  }

  for (driver* curr_driver = drivers_head; curr_driver < drivers_tail; curr_driver += 1) {
    if (strcmp(name_local, curr_driver->name) == 0) {
      return curr_driver;
    }
  }
  return NULL;
}

int init_driver(char *name) {
  driver *d = get_driver_from_name(name);
  if(d) {
    if(d->init) {
      return d->init();
    } else {
      return 0;
    }
  }
  else return -1;
}

int fini_driver(char *name) {
  driver *d = get_driver_from_name(name);
  if(d) {
    if(d->fini) {
      return d->fini();
    } else {
      return 0;
    }
  }
  else return -1;
}

uintptr_t drivers_openat_override(int dirfd, char* path, int flags, mode_t mode) {
  if (dirfd == EYRIE_DEVICES_DIRFD) {
    char driver_name[DRIVER_NAME_MAX_LEN];
    copy_from_user((void*)driver_name, (void*)path, DRIVER_NAME_MAX_LEN);

    driver* matching_driver = get_driver_from_name(driver_name);
    if (matching_driver == NULL) return -1;

    // find the first open fd node slot
    for (int fd = 0; fd < FD_COUNT; fd += 1) {
      if (fd_nodes[fd] == NULL) {
        fd_nodes[fd] = matching_driver;
        return fd + EYRIE_FD_OFFSET;
      }
    }

    return -1;
  } else {
#ifdef USE_IO_SYSCALL
    return io_syscall_openat(dirfd, path, flags, mode);
#else
    return -1;
#endif
  }
}

uintptr_t drivers_close_override(int fd) {
  if(fd >= EYRIE_FD_OFFSET) {
    fd_nodes[fd - EYRIE_FD_OFFSET] = NULL;
    return 0;
  } else {
#ifdef USE_IO_SYSCALL
    return io_syscall_close(fd);
#else
    return -1;
#endif
  }
}

uintptr_t drivers_read_override(int fd, void* buf, size_t len) {
  driver *d;
  if (fd >= EYRIE_FD_OFFSET) {
    d = fd_nodes[fd - EYRIE_FD_OFFSET];

    if(d->read) {
      return d->read(buf, len);
    } else {
      return 0;
    }
  } else {
#ifdef USE_IO_SYSCALL
    return io_syscall_read(fd, buf, len);
#else
    return -1;
#endif
  }
}

uintptr_t drivers_write_override(int fd, void* buf, size_t len) {
  driver *d;
  if (fd >= EYRIE_FD_OFFSET) {
    d = fd_nodes[fd - EYRIE_FD_OFFSET];

    if(d->write) {
      return d->write(buf, len);
    } else {
      return 0;
    }
  } else {
#ifdef USE_IO_SYSCALL
    return io_syscall_write(fd, buf, len);
#else
    return -1;
#endif
  }
}

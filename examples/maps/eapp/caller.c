
#include <stdint.h>
#include <stdio.h>

#include "app/export.h"
#include "app/syscall.h"
#include "app/callee.h"

extern struct export_function __start_keystone_exported;
extern struct export_function __stop_keystone_exported;

int
main() {
  int ret;
  struct export_function* func;

  for (func = &__start_keystone_exported; func != &__stop_keystone_exported;
       func++) {

    ret = share_region(func->addr, func->size, 0);
    if (ret < 0) {
      printf("Failed to share region\n");
      return ret;
    }

    // Synchronous call first
    ret = call_enclave(0, CALL_MAPPED, 4,
                       translate(func->addr), func->size, func->base_va, 5);
    if(ret < 0) {
      printf("Failed to call enclave\n");
      return ret;
    }

    ret = unshare_region(func->addr, 0);
    if (ret < 0) {
      printf("Failed to unshare region\n");
      return ret;
    }
  }
}
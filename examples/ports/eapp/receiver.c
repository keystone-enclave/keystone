
#include <errno.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "app/callee.h"
#include "app/syscall.h"

#define SECURE_DEVICE  "uart@10001000"

volatile static pthread_spinlock_t lock;
volatile static bool cmd_valid = false;
volatile static uint8_t cmd = 0;

int uart_handler(void *arg) {
  bool should_ret = false;
  uint64_t *args = arg;

  // spin until we reset
  do {
    if(pthread_spin_trylock(&lock) == EBUSY) {
      yield_thread();
      continue;
    }

    if(!cmd_valid) {
      cmd_valid = true;
      cmd = args[0] & 0xFF;
      should_ret = true;
    }

    pthread_spin_unlock(&lock);

    // Don't yield unless we need to
    if(!should_ret)
      yield_thread();
  } while(!should_ret);

  return 0;
}

int main() {
  bool done = false;
  int ret;

  ret = claim_mmio(SECURE_DEVICE,
                   strlen(SECURE_DEVICE));
  if(ret < 0) {
    printf("Failed to claim " SECURE_DEVICE "\n");
    return -1;
  }

  ret = init_callee();
  if(ret < 0) {
    printf("Failed to initialize callee subsystem\n");
    return -1;
  }

  ret = spawn_callee_handler(uart_handler, CALL_RECEIVER);
  if(ret < 0) {
    printf("Failed to start callee handler\n");
    return -1;
  }

  // Spin
  do {
    if(pthread_spin_lock(&lock) == EBUSY) {
      yield_thread();
      continue;
    }

    if(cmd_valid) {
      if(cmd == 0xFF)
        done = true;
      else if(cmd != 0) {
        printf("%c", cmd);
        fflush(stdout);
      }

      cmd_valid = false;
    }

    pthread_spin_unlock(&lock);
    yield_thread();
  } while(!done);

  ret = release_mmio(SECURE_DEVICE,
                     strlen(SECURE_DEVICE));
  if(ret < 0) {
    printf("Failed to release " SECURE_DEVICE "\n");
    return -1;
  }
}
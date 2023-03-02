
#include <stdio.h>
#include <string.h>

#include "app/syscall.h"
#include "app/callee.h"

#define SECURE_DEVICE  "uart@10001000"

int main()
{
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

  ret = spawn_callee_handler(NULL, CALL_MAPPED);
  if(ret < 0) {
    printf("Failed to start callee handler\n");
    return -1;
  }

  while(1) {
    yield_thread();
  }
}
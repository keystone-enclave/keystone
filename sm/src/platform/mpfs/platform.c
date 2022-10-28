/* Default platform does nothing special here */
#include "platform-hook.h"
#include "../../enclave.h"
#include "../../../plat/mpfs/drivers/mss_sys_services/mss_sys_services.h"
#include <sbi/riscv_locks.h>
#include <assert.h>

unsigned long platform_init_global_once(void){
  return SBI_ERR_SM_ENCLAVE_SUCCESS;
}

unsigned long platform_init_global(void){
  return SBI_ERR_SM_ENCLAVE_SUCCESS;
}

void platform_init_enclave(struct enclave* enclave){
  return;
}

void platform_destroy_enclave(struct enclave* enclave){
  return;
}

unsigned long platform_create_enclave(struct enclave* enclave){
  return SBI_ERR_SM_ENCLAVE_SUCCESS;
}

void platform_switch_to_enclave(struct enclave* enclave){
  return;
}

void platform_switch_from_enclave(struct enclave* enclave){
  return;
}

#define NONCE_BYTES (256/8)
#define RAND_STATE_WORDS (NONCE_BYTES/sizeof(uint64_t))
struct random {
  uint64_t data[RAND_STATE_WORDS];
  uint8_t words_left;
};
static spinlock_t rand_state_lock = SPIN_LOCK_INITIALIZER;
static struct random rand_state;

uint64_t platform_random(void){
  spin_lock(&rand_state_lock);

  if (!rand_state.words_left) {
    // This service provides 256 bits of real random data
    // It might be a bit slow to query though.
    int ret = MSS_SYS_nonce_service((uint8_t *)&rand_state.data, 0);
    sm_assert(ret == 0);
    rand_state.words_left = RAND_STATE_WORDS;
  }
  uint64_t out = rand_state.data[--rand_state.words_left];

  spin_unlock(&rand_state_lock);
  return out;
}

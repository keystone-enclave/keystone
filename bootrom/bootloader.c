#include <stddef.h>

#define ED25519_NO_SEED 1
#include "sha3/sha3.h"
/* Adopted from https://github.com/orlp/ed25519
  provides:
  - void ed25519_create_keypair(t_pubkey *public_key, t_privkey *private_key, t_seed *seed);
  - void ed25519_sign(t_signature *signature,
                      const unsigned uint8_t *message,
                      size_t message_len,
                      t_pubkey *public_key,
                      t_privkey *private_key);
*/

#include "ed25519/ed25519.h"
/* adopted from
  provides:
  - int sha3_init(sha3_context * md);
  - int sha3_update(sha3_context * md, const unsigned char *in, size_t inlen);
  - int sha3_final(sha3_context * md, unsigned char *out);
  types: sha3_context
*/

#include "string.h"
/*
  provides memcpy, memset
*/


typedef unsigned char byte;

// Sanctum header fields in DRAM
extern byte sanctum_dev_public_key[32];
extern byte sanctum_dev_secret_key[64];
unsigned int sanctum_sm_size = 0x2ff000;
extern byte sanctum_sm_hash[64];
extern byte sanctum_sm_public_key[32];
extern byte sanctum_sm_secret_key[64];
extern byte sanctum_sm_signature[64];
#define DRAM_BASE 0x80000000

inline byte random_byte() {
	return 0xda;
}

void bootloader() {
	//*sanctum_sm_size = 0x200;
  // Reserve stack space for secrets
  byte scratchpad[128];
  sha3_ctx_t hash_ctx;

  // TODO: on real device, copy boot image from memory. In simulator, HTIF writes boot image
  // ... SD card to beginning of memory.
  // sd_init();
  // sd_read_from_start(DRAM, 1024);

  // Create a random seed for a device key from TRNG
  for (unsigned int i=0; i<32; i++) {
    scratchpad[i] = 0xac + (0xdd ^ i);//random_byte();
  }

  // Derive {SK_D, PK_D} (device keys) from a 32 B random seed
  ed25519_create_keypair(sanctum_dev_public_key, sanctum_dev_secret_key, scratchpad);

  // Measure SM
  sha3_init(&hash_ctx, 64);
  sha3_update(&hash_ctx, (void*)DRAM_BASE, sanctum_sm_size);
  sha3_final(sanctum_sm_hash, &hash_ctx);

  // Combine SK_D and H_SM via a hash
  // sm_key_seed <-- H(SK_D, H_SM), truncate to 32B
  sha3_init(&hash_ctx, 64);
  sha3_update(&hash_ctx, sanctum_dev_secret_key, sizeof(*sanctum_dev_secret_key));
  sha3_update(&hash_ctx, sanctum_sm_hash, sizeof(*sanctum_sm_hash));
  sha3_final(scratchpad, &hash_ctx);
  // Derive {SK_D, PK_D} (device keys) from the first 32 B of the hash (NIST endorses SHA512 truncation as safe)
  ed25519_create_keypair(sanctum_sm_public_key, sanctum_sm_secret_key, scratchpad);

  // Endorse the SM
  memcpy(scratchpad, sanctum_sm_hash, 64);
  memcpy(scratchpad + 64, sanctum_sm_public_key, 32);
  // Sign (H_SM, PK_SM) with SK_D
  ed25519_sign(sanctum_sm_signature, scratchpad, 64 + 32, sanctum_dev_public_key, sanctum_dev_secret_key);

  // Clean up
  // Erase SK_D
  memset((void*)sanctum_dev_secret_key, 0, sizeof(*sanctum_sm_secret_key));

  // caller will clean core state and memory (including the stack), and boot.
  return;
}

/*
 * keystone.c
 */

#include <common.h>
#include <stddef.h>
#include <string.h>
#include <asm/keystone.h>

#define ED25519_NO_SEED 1
#include "sha3/sha3.h"
#include "ed25519/ed25519.h"

typedef unsigned char byte;

/* Sanctum fields */
extern byte sanctum_dev_public_key[32];
extern byte sanctum_dev_secret_key[64];
unsigned int sanctum_sm_size = 0x1ff000;
extern byte sanctum_sm_hash[64];
extern byte sanctum_sm_public_key[32];
extern byte sanctum_sm_secret_key[64];
extern byte sanctum_sm_signature[64];

#if CONFIG_IS_ENABLED(KEYSTONE_USE_RNG)

#error Random function using hardware rng is unimplemented

#else

#warning Bootloader does not have entropy source, keys are for TESTING ONLY

inline byte random_byte(unsigned int i) {
    return 0xac + (0xdd ^ i);
}

#endif

int keystone_init(void) {
    byte scratchpad[128];
    sha3_ctx_t hash_ctx;

    for (unsigned int i = 0; i < 32; i++) {
        scratchpad[i] = random_byte(i);
    }

#if CONFIG_IS_ENABLED(KEYSTONE_USE_TEST_KEYS)
    #include "keystone_use_test_keys.h"
#else
    #error Secure keystore operation is unimplemented
#endif

    /* Measure SM */
    sha3_init(&hash_ctx, 64);
    sha3_update(&hash_ctx, (void *)CONFIG_SPL_OPENSBI_LOAD_ADDR, sanctum_sm_size);
    sha3_final(sanctum_sm_hash, &hash_ctx);

    /* Combine SK_D and H_SM via a hash
     * sm_key_seed <-- H(SK_D, H_SM), truncate to 32B */
    sha3_init(&hash_ctx, 64);
    sha3_update(&hash_ctx, sanctum_dev_secret_key, sizeof(*sanctum_dev_secret_key));
    sha3_update(&hash_ctx, sanctum_sm_hash, sizeof(*sanctum_sm_hash));
    sha3_final(scratchpad, &hash_ctx);

    /* Derive {SK_D, PK_D} (device keys) from the first 32 B of the hash (NIST endorses SHA512 truncation as safe) */
    ed25519_create_keypair(sanctum_sm_public_key, sanctum_sm_secret_key, scratchpad);

    /* Endorse the SM */
    memcpy(scratchpad, sanctum_sm_hash, 64);
    memcpy(scratchpad + 64, sanctum_sm_public_key, 32);
    /* Sign (H_SM, PK_SM) with SK_D */
    ed25519_sign(sanctum_sm_signature, scratchpad, 64 + 32, sanctum_dev_public_key, sanctum_dev_secret_key);

    /* Clean up
     * Erase SK_D */
    memset((void *)sanctum_dev_secret_key, 0, sizeof(*sanctum_dev_secret_key));

    /* caller will clean core state and memory (including the stack). */
    return 0;
}

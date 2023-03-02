#include "ed25519.h"
#include "crypto/hash.h"
#include "ge.h"


void ed25519_create_keypair(unsigned char *public_key, unsigned char *private_key, const unsigned char *seed) {
    ge_p3 A;

    //    sha3(seed, 32, private_key, 64);
    crypto_hash_data(seed, 32, private_key, CRYPTO_SHA3);
    private_key[0] &= 248;
    private_key[31] &= 63;
    private_key[31] |= 64;

    ge_scalarmult_base(&A, private_key);
    ge_p3_tobytes(public_key, &A);
}

#include "ed25519.h"
#include "sha3/sha3.h"
#include "ge.h"
#include "sc.h"


void ed25519_sign(unsigned char *signature, const unsigned char *message, size_t message_len, const unsigned char *public_key, const unsigned char *private_key) {
    sha3_ctx_t hash;
    unsigned char hram[64];
    unsigned char r[64];
    ge_p3 R;


    sha3_init(&hash, 64);
    sha3_update(&hash, private_key + 32, 32);
    sha3_update(&hash, message, message_len);
    sha3_final(r, &hash);

    sc_reduce(r);
    ge_scalarmult_base(&R, r);
    ge_p3_tobytes(signature, &R);

    sha3_init(&hash, 64);
    sha3_update(&hash, signature, 32);
    sha3_update(&hash, public_key, 32);
    sha3_update(&hash, message, message_len);
    sha3_final(hram, &hash);

    sc_reduce(hram);
    sc_muladd(signature + 32, hram, private_key, r);
}

#include "crypto/hash.h"
#include "ed25519.h"
//#include "sha3/sha3.h"
#include "ge.h"
#include "sc.h"

void ed25519_sign(unsigned char *signature, const unsigned char *message,
                  size_t message_len, const unsigned char *public_key,
                  const unsigned char *private_key) {
  crypto_sha256_ctx hash;
  unsigned char hram[64];
  unsigned char r[64];
  ge_p3 R;

  crypto_sha256_init(&hash);
  crypto_sha256_update(&hash, private_key + 32, 32);
  crypto_sha256_update(&hash, message, message_len);
  crypto_sha256_final(&hash, r);

  sc_reduce(r);
  ge_scalarmult_base(&R, r);
  ge_p3_tobytes(signature, &R);

  crypto_sha256_init(&hash);
  crypto_sha256_update(&hash, signature, 32);
  crypto_sha256_update(&hash, public_key, 32);
  crypto_sha256_final(&hash, hram);

  sc_reduce(hram);
  sc_muladd(signature + 32, hram, private_key, r);
}

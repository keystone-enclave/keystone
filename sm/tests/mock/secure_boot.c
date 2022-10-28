#include "crypto.h"

byte __wrap_sanctum_sm_hash[MDSIZE];
byte __wrap_sanctum_sm_signature[SIGNATURE_SIZE];
byte __wrap_sanctum_sm_public_key[PRIVATE_KEY_SIZE];
byte __wrap_sanctum_sm_secret_key[PUBLIC_KEY_SIZE];
byte __wrap_sanctum_dev_public_key[PUBLIC_KEY_SIZE];


#ifndef SM_ENCLAVE_SYSCALL
#define SM_ENCLAVE_SYSCALL

#include <stdint.h>

uint64_t sm_fetch_field(void* out_field, uint64_t field_id);

uint64_t sm_aes_cbc(void* buffer, uint8_t iv[16], uint32_t buffer_len);

uint64_t sm_sign_message(uint8_t out_signature[64], void* in_message, uint32_t message_len);

uint64_t sm_poet(uint8_t * out_hmac, uint8_t * out_signature, uint8_t * in_message, uint32_t in_message_len);


#endif


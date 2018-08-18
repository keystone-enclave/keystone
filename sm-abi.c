#include "mcall.h"
#include "mtrap.h"
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include "sm-abi.h"
#include "enclave.h"

#define AES256 1
#define CTR 1
#include "aes/aes.h"

#define ED25519_NO_SEED 1
#include "ed25519/ed25519.h"

#include "sha3/sha3.h"

extern uint8_t PK_D[32];
extern uint8_t SM_H[64];
extern uint8_t PK_SM[32];
extern uint8_t SK_SM[64];
extern uint8_t SM_SIG[64];


void * virt_to_phys(uint64_t va) {
  // Get the page table base pointer
  // Assume Sv39

  // Level 2
  uint64_t * PT2 = (uint64_t*)((read_csr(0x180) & 0xFFFFFFFFFFF) << 12); // SATP: low 44 bits are the PPN
  int vpn2 = (va >> 30) & 0x1FF;
  uint64_t pte2 = PT2[vpn2];
  uint64_t ppn2 = (pte2 >> 10);
  char xwrv2 = pte2 & 0xF;
  if ((xwrv2 & 0x1) == 0) { // Invalid
    return 0;
  } else if (xwrv2 != 1) { // Leaf; giga page
    pte2 |= 0x80;
    PT2[vpn2] = pte2; // TODO: this update must be atomic in general
    uint64_t mask2 = (0xFFFFFFC0000) << 12;
    uint64_t pa = (va & ~(mask2)) | ((ppn2 << 12) & mask2);
    return (void*)pa;
  }

  // Level 1
  uint64_t * PT1 = (uint64_t*)(ppn2 << 12);
  int vpn1 = (va >> 21) & 0x1FF;
  uint64_t pte1 = PT1[vpn1];
  uint64_t ppn1 = (pte1 >> 10);
  char xwrv1 = pte1 & 0xF;
  if ((xwrv1 & 0x1) == 0) { // Invalid
    return 0;
  } else if (xwrv1 != 1) { // Leaf; mega page
    pte1 |= 0x80;
    PT1[vpn1] = pte1; // TODO: this update should be atomic
    uint64_t mask1 = (0xFFFFFFFFE00) << 12;
    uint64_t pa = (va & ~(mask1)) | ((ppn1 << 12) & mask1);
    return (void*)pa;
  }

  // Level 0
  uint64_t * PT0 = (uint64_t*)(ppn1 << 12);
  int vpn0 = (va >> 12) & 0x1FF;
  uint64_t pte0 = PT0[vpn0];
  uint64_t ppn0 = (pte0 >> 10);
  char xwrv0 = pte0 & 0xF;
  if ((xwrv0 & 0x1) == 0) { // Invalid
    return 0;
  }

  // Set dirty bit
  pte0 |= 0x80;
  PT0[vpn0] = pte0; // TODO: this update should be atomic
  uint64_t mask0 = (0xFFFFFFFFFFF) << 12;
  uint64_t pa = (va & ~(mask0)) | ((ppn0 << 12) & mask0);
  return (void*)pa;

  // TODO: probably wise to use MSTATUS to ensure OS permissions are not ignored during SM syscalls
}

void u_ecall_trap(uintptr_t* regs, uintptr_t mcause, uintptr_t mepc)
{
  write_csr(mepc, mepc + 4);

  uintptr_t n = regs[17], arg0 = regs[10], arg1 = regs[11], arg2 = regs[12], arg3 = regs[13], retval, ipi_type;

  printm("user ecall n:%d\n",n);
  switch (n)
  {
    // Security monitor calls
    case ABI_SM_DEADBEEF:
      retval = 0xDEADBEEF;
      break;
    case ABI_SM_GET_FIELD:
      retval = sm_fetch_field( (void*)virt_to_phys(arg0), (uint64_t)arg1 );
      break;
    case ABI_SM_AES:
      retval = sm_aes_cbc( (void*)virt_to_phys(arg0), (uint8_t*)virt_to_phys(arg1), (uint32_t)arg2 );
      break;
    case ABI_SM_SIGN:
      retval = sm_sign_message( (uint8_t*)virt_to_phys(arg0), (void*)virt_to_phys(arg1), (uint32_t)arg2 );
      break;
    case ABI_SM_POET:
      retval = sm_poet( (uint8_t*)virt_to_phys(arg0), (uint8_t*)virt_to_phys(arg1), (uint8_t*)virt_to_phys(arg2), (uint32_t)arg3 );
      break;
    default:
      retval = -ENOSYS;
      break;
  }
  regs[10] = retval;
}

uint64_t sm_fetch_field(void* out_field, uint64_t field_id) {
  uint64_t result = 0;
  switch (field_id) {
    case SM_FIELD_PK_D:
      memcpy(out_field, PK_D, 32);
      break;
    case SM_FIELD_H_SM:
      memcpy(out_field, SM_H, 64);
      break;
    case SM_FIELD_PK_SM:
      memcpy(out_field, PK_SM, 32);
      break;
    case SM_FIELD_SIGN_D:
      memcpy(out_field, SM_SIG, 64);
      break;
    default:
      // no effect
      result = -ENOSYS;
      break;
  }
  return result;
}

uint64_t sm_aes_cbc(void* buffer, uint8_t iv[16], uint32_t buffer_len) {
  if (buffer_len > 512) {
    return -ENOSYS;
  }

  uint8_t sm_iv[16];
  uint8_t sm_buffer[512];
  uint8_t sm_symkey[32];

  memcpy(sm_buffer, buffer, buffer_len);
  memcpy(sm_iv, iv, 12);
  sha3(SK_SM, 64, sm_symkey, 32);

  struct AES_ctx aes_context;
  AES_init_ctx_iv(&aes_context, sm_symkey, sm_iv);
  AES_CTR_xcrypt_buffer(&aes_context, sm_buffer, buffer_len);

  memcpy(buffer, sm_buffer, buffer_len);

  return 0;
}

uint64_t sm_sign_message(uint8_t out_signature[64], void* in_message, uint32_t message_len) {
  if (message_len > 512) {
    return -ENOSYS;
  }

  uint8_t sm_signature[64];
  uint8_t sm_message[512];

  memcpy(sm_message, in_message, message_len);
  ed25519_sign(sm_signature, sm_message, message_len, PK_SM, SK_SM);
  memcpy(out_signature, sm_signature, 64);

  return 0;
}

uint64_t sm_poet(uint8_t * out_hmac, uint8_t * out_signature, uint8_t * in_message, uint32_t in_message_len) {
  if (in_message_len > 512) {
    return -ENOSYS;
  }

  uint8_t sm_message[512];
  uint8_t sm_hash[32];
  uint8_t sm_signature[64];

  memcpy(sm_message, in_message, in_message_len);

  // HMAC
  sha3_ctx_t hash_ctx;
  sha3_init(&hash_ctx, 32);
  sha3_update(&hash_ctx, sm_message, in_message_len);
  sha3_update(&hash_ctx, SK_SM, 64);
  sha3_final(sm_hash, &hash_ctx);

  // Elapsed time
  for (int i=0; i<0xFFF; i++) {
    read_csr(0xCC0);
  }

  // Signature
  ed25519_sign(sm_signature, sm_hash, 32, PK_SM, SK_SM);

  memcpy(out_hmac, sm_hash, 32);
  memcpy(out_signature, sm_signature, 64);

  return 0;
}

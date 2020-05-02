/* Default platform does nothing special here */
#include "enclave.h"

enclave_ret_code platform_init_global_once(){
  return ENCLAVE_SUCCESS;
}

enclave_ret_code platform_init_global(){
  return ENCLAVE_SUCCESS;
}

void platform_init_enclave(struct enclave* enclave){
  return;
}

void platform_destroy_enclave(struct enclave* enclave){
  return;
}

enclave_ret_code platform_create_enclave(struct enclave* enclave){
  return ENCLAVE_SUCCESS;
}

void platform_switch_to_enclave(struct enclave* enclave){
  return;
}

void platform_switch_from_enclave(struct enclave* enclave){
  return;
}

uint64_t platform_random(){
#pragma message("Platform has no entropy source, this is unsafe. TEST ONLY")
  static uint64_t w = 0, s = 0xb5ad4eceda1ce2a9;

  unsigned long cycles;
  asm volatile ("rdcycle %0" : "=r" (cycles));
  
  // from Middle Square Weyl Sequence algorithm
  uint64_t x = cycles;
  x *= x; 
  x += (w += s); 
  return (x>>32) | (x<<32);
}

/* Default platform does nothing special here */
#include "enclave.h"

void platform_init(struct platform_enclave_data_t* enclave){
  return;
}

void platform_switch_to_enclave(struct platform_enclave_data_t* enclave){
  return;
}

void platform_switch_from_enclave(struct platform_enclave_data_t* enclave){
  return;
}

uint64_t platform_random(){
#pragma message("Platform has no entropy source, this is unsafe. TEST ONLY")
  unsigned long cycles;
  asm volatile ("rdcycle %0" : "=r" (cycles));
  return cycles;
}

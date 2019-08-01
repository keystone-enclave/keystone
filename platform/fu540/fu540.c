#include "fu540_internal.c"
#include "waymasks.c"

uint64_t platform_random(){
#pragma message("Platform has no entropy source, this is unsafe. TEST ONLY")
  unsigned long cycles;
  asm volatile ("rdcycle %0" : "=r" (cycles));
  return cycles;
}

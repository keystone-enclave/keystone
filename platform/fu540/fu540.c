#include "fu540_internal.c"
#include "waymasks.c"

void platform_getrandom_fill(uint8_t* buffer, unsigned long size){

#pragma message("Platform has no entropy source, this is unsafe. TEST ONLY")
  unsigned long cycles;
  while(size > 0){
    asm volatile ("rdcycle %0" : "=r" (cycles));
    *buffer = cycles % 255;
    size--;
    buffer++;
  }
}

#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>
<<<<<<< HEAD
#include <stddef.h>
=======

void* memcpy2(void* dest, const void* src, size_t len)
{
  const char* s = src;
  char *d = dest;
>>>>>>> dev-rohit-sha256-fix

void* memcpy(void* dest, const void* src, size_t len);

//void* memset(void* dest, int byte, size_t len);

<<<<<<< HEAD
void* memset2(void* dest, int byte, size_t len);
=======
  return dest;
}

void* memset2(void* dest, int byte, size_t len)
{
  if ((((uintptr_t)dest | len) & (sizeof(uintptr_t)-1)) == 0) {
    uintptr_t word = byte & 0xFF;
    word |= word << 8;
    word |= word << 16;
    word |= word << 16 << 16;

    uintptr_t *d = dest;
    while (d < (uintptr_t*)(dest + len))
      *d++ = word;
  } else {
    char *d = dest;
    while (d < (char*)(dest + len))
      *d++ = byte;
  }
  return dest;
}
>>>>>>> dev-rohit-sha256-fix

#endif

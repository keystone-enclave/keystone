#ifndef __STRING_H__
#define __STRING_H__

/* TODO: Use actual implementation of these functions. */

void* memcpy(void* dest, const void* src, unsigned int len)
{
  const char* s = src;
  char *d = dest;
  int i;
  for (i = 0; i < len; ++i) {
    d[i] = s[i];
  }
  return d;
}

void* memset(void* dest, int byte, unsigned int len)
{
  char* d = dest;
  char* end = dest + len;
  while (d != end) {
    *d++ = byte;
  }
  return dest;
}


#endif

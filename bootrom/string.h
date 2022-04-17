#ifndef __STRING_H__
#define __STRING_H__

#include <stdint.h>
#include <stddef.h>

void* memcpy(void* dest, const void* src, size_t len);

//void* memset(void* dest, int byte, size_t len);

void* memset2(void* dest, int byte, size_t len);

#endif

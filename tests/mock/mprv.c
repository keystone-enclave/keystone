#include <stdint.h>

int __wrap_copy1_to_sm(uint8_t *dst, uint8_t *src)
{
    *dst = *src;
    return 0;
}

int __wrap_copy8_to_sm(uint64_t *dst, uint64_t *src)
{
    *dst = *src;
    return 0;
}

int __wrap_copy64_to_sm(uint64_t *dst, uint64_t *src)
{
    for (int i = 0; i < 8; i++)
        *dst++ = *src++;
    return 0;
}

int __wrap_copy1_from_sm(uint8_t *dst, uint8_t *src)
{
    *dst = *src;
    return 0;
}

int __wrap_copy8_from_sm(uint64_t *dst, uint64_t *src)
{
    *dst = *src;
    return 0;
}

int __wrap_copy64_from_sm(uint64_t *dst, uint64_t *src)
{
    for (int i = 0; i < 8; i++)
        *dst++ = *src++;
    return 0;
}

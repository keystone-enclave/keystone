#pragma once
#include <stdint.h>
#include <stdbool.h>

int copy1_from_sm(uintptr_t dst, const uint8_t *src);
int copy8_from_sm(uintptr_t dst, const uint64_t *src);
int copy64_from_sm(uintptr_t dst, const uint64_t *src);

int copy1_to_sm(uint8_t *dst, uintptr_t src);
int copy8_to_sm(uint64_t *dst, uintptr_t src);
int copy64_to_sm(uint64_t *dst, uintptr_t src);

int copy_from_sm(uintptr_t dst, void *src_buf, size_t len)
{
    uintptr_t src = (uintptr_t)src_buf;

    if (src % 8 == 0 && dst % 8 == 0) {
        while (len >= 64) {
            int res = copy64_from_sm(dst, (uint64_t *)src);
            if (res)
                return res;
            
            src += 64;
            dst += 64;
            len -= 64;
        }

        while (len >= 8) {
            int res = copy8_from_sm(dst, (uint64_t *)src);
            if (res)
                return res;

            src += 8;
            dst += 8;
            len -= 8;
        }
    }

    while (len > 0) {
        int res = copy1_from_sm(dst, (uint8_t *)src);
        if (res)
            return res;

        src++;
        dst++;
        len--;
    }

    return 0;
}

int copy_to_sm(void *dst_buf, uintptr_t src, size_t len)
{
    uintptr_t dst = (uintptr_t)dst_buf;

    if (src % 8 == 0 && dst % 8 == 0) {
        while (len >= 64) {
            int res = copy64_to_sm((uint64_t *)dst, src);
            if (res)
                return res;
            
            src += 64;
            dst += 64;
            len -= 64;
        }

        while (len >= 8) {
            int res = copy8_to_sm((uint64_t *)dst, src);
            if (res)
                return res;

            src += 8;
            dst += 8;
            len -= 8;
        }
    }

    while (len > 0) {
        int res = copy1_to_sm((uint8_t *)dst, src);
        if (res)
            return res;

        src++;
        dst++;
        len--;
    }

    return 0;
}

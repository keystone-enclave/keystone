#pragma once
#include <sbi/sbi_types.h>

typedef struct { size_t words[8]; } mprv_block;

int copy1_from_sm(uintptr_t dst, const uint8_t *src);
int copy_word_from_sm(uintptr_t dst, const uintptr_t *src);
int copy_block_from_sm(uintptr_t dst, const mprv_block *src);

int copy1_to_sm(uint8_t *dst, uintptr_t src);
int copy_word_to_sm(uintptr_t *dst, uintptr_t src);
int copy_block_to_sm(mprv_block *dst, uintptr_t src);

#if __riscv_xlen == 64
# define STORE    sd
# define LOAD     ld
# define LOG_REGBYTES 3
#elif __riscv_xlen == 32
# define STORE    sw
# define LOAD     lw
# define LOG_REGBYTES 2
#endif

#define REGBYTES (1 << LOG_REGBYTES)
#define MPRV_BLOCK (REGBYTES * 8)

static inline int copy_from_sm(uintptr_t dst, void *src_buf, size_t len)
{
    uintptr_t src = (uintptr_t)src_buf;

    if (src % REGBYTES  == 0 && dst % REGBYTES == 0) {
        while (len >= MPRV_BLOCK) {
            int res = copy_block_from_sm(dst, (mprv_block *)src);
            if (res)
                return res;

            src += MPRV_BLOCK;
            dst += MPRV_BLOCK;
            len -= MPRV_BLOCK;
        }

        while (len >= REGBYTES) {
            int res = copy_word_from_sm(dst, (uintptr_t *)src);
            if (res)
                return res;

            src += REGBYTES;
            dst += REGBYTES;
            len -= REGBYTES;
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

static inline int copy_to_sm(void *dst_buf, uintptr_t src, size_t len)
{
    uintptr_t dst = (uintptr_t)dst_buf;

    if (src % REGBYTES == 0 && dst % REGBYTES == 0) {
        while (len >= MPRV_BLOCK) {
            int res = copy_block_to_sm((mprv_block *)dst, src);
            if (res)
                return res;

            src += MPRV_BLOCK;
            dst += MPRV_BLOCK;
            len -= MPRV_BLOCK;
        }

        while (len >= REGBYTES) {
            int res = copy_word_to_sm((uintptr_t *)dst, src);
            if (res)
                return res;

            src += REGBYTES;
            dst += REGBYTES;
            len -= REGBYTES;
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

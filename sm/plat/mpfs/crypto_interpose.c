
#include <crypto.h>
#include "page.h"

#include <sbi_string.h>

#include <sig/sig_algs.h>

void hash_init(hash_ctx* ctx) {
//        sha512_init(ctx);
}

void hash_extend(hash_ctx* ctx, const void* ptr, size_t len) {
//        sha512_update(ctx, ptr, len);
}

void hash_extend_page(hash_ctx* ctx, const void* ptr) {
//        sha512_update(ctx, ptr, RISCV_PGSIZE);
}

void hash_finalize(void* md, hash_ctx* ctx) {
//        sha512_final(ctx, md);

        // For now, just set to 0
        sbi_memset(md, 0, MDSIZE);
}

void sign(void* sign, const void* data, size_t len, const byte* public_key, const byte* private_key) {
        // For now, just set to 0
        sbi_memset(sign, 0, SIGNATURE_SIZE);
}

int kdf(const unsigned char* salt, size_t salt_len,
        const unsigned char* ikm, size_t ikm_len,
        const unsigned char* info, size_t info_len,
        unsigned char* okm, size_t okm_len) {
        // For now, just set to 0
        sbi_memset(sign, 0, okm_len);
        return 0;
}


set(CRYPTO_SOURCES "")

if(PAGE_CRYPTO)
    list(APPEND CRYPTO_SOURCES aes.c)
endif()

if(PAGE_HASH)
    list(APPEND CRYPTO_SOURCES sha256.c merkle.c)
endif()

if(NOT CRYPTO_SOURCES)
    list(APPEND CRYPTO_SOURCES ../util/empty.c)
endif()

add_library(rt_crypto ${CRYPTO_SOURCES})
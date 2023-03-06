cmake_minimum_required(VERSION 3.0)
project(eyrie_test)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmocka/cmake/Modules")

set(WITH_EXAMPLES false)
add_subdirectory(cmocka)

include(AddCMockaTest)
enable_testing()

include_directories(../include)

add_cmocka_test(test_string SOURCES string.c COMPILE_OPTIONS -I${CMAKE_BINARY_DIR}/cmocka/include LINK_LIBRARIES cmocka)
add_cmocka_test(test_merkle
    SOURCES merkle.c ../crypto/sha256.c
    COMPILE_OPTIONS -DUSE_PAGE_HASH -DUSE_PAGING -DUSE_FREEMEM -D__riscv_xlen=64 -I${CMAKE_BINARY_DIR}/cmocka/include -g
    LINK_LIBRARIES cmocka)
add_cmocka_test(test_pageswap
    SOURCES page_swap.c ../crypto/merkle.c ../crypto/sha256.c ../crypto/aes.c
    COMPILE_OPTIONS -DUSE_PAGE_HASH -DUSE_PAGE_CRYPTO -DUSE_PAGING -DUSE_FREEMEM -D__riscv_xlen=64 -I${CMAKE_BINARY_DIR}/cmocka/include -g
    LINK_LIBRARIES cmocka)


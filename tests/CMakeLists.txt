cmake_minimum_required(VERSION 3.12)
project(keystone_test C)

if (RISCV32)
  SET(CMAKE_C_COMPILER riscv32-unknown-linux-gnu-gcc)
  SET(QEMU qemu-riscv32)
  SET(LIBCMOCKA ../cmocka/libcmocka-static-32.a)
else ()
  SET(CMAKE_C_COMPILER riscv64-unknown-linux-gnu-gcc)
  SET(QEMU qemu-riscv64)
  SET(LIBCMOCKA ../cmocka/libcmocka-static.a)
endif()

SET(CMAKE_C_FLAGS -static)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ./cmocka/
    ../../machine
    ../../
    ../../build
    ../
)
enable_testing()
SET(CMOCKA_LIBRARY ${LIBCMOCKA})
add_library(cmocka STATIC IMPORTED)
SET_TARGET_PROPERTIES(cmocka PROPERTIES IMPORTED_LOCATION ${CMOCKA_LIBRARY})

### test pmp ###
add_executable(test_pmp test_pmp.c mock/machine.c)
target_link_libraries(test_pmp cmocka)
add_test(test_pmp
        ${QEMU} ${CMAKE_CURRENT_BINARY_DIR}/test_pmp)
set_target_properties(test_pmp
        PROPERTIES
        LINK_FLAGS  "-Wl,--wrap=printm -Wl,--wrap=poweroff -Wl,--wrap=disabled_hart_mask -Wl,--wrap=hart_mask"
)

### test enclave ###
add_executable(test_enclave
                test_enclave.c
                mock/machine.c
                mock/mprv.c
                mock/secure_boot.c
                ../platform/default/default.c
                ../sha3/sha3.c
                ../ed25519/ge.c
                ../ed25519/fe.c
                ../ed25519/sc.c
                ../ed25519/sign.c
                ../ed25519/keypair.c
                ../hkdf_sha3_512/hkdf_sha3_512.c
                ../hmac_sha3/hmac_sha3.c
                ../pmp.c
                ../attest.c
                ../cpu.c
                ../crypto.c
                ../thread.c
                ../sm.c
                )
target_link_libraries(test_enclave cmocka)
add_test(test_enclave
        ${QEMU} ${CMAKE_CURRENT_BINARY_DIR}/test_enclave)
set_target_properties(test_enclave
        PROPERTIES
        COMPILE_FLAGS -DTARGET_PLATFORM_HEADER=\\"..\/platform\/default\/default.h\\"
        LINK_FLAGS
          "-Wl,--wrap=printm \
  -Wl,--wrap=mtime   \
          -Wl,--wrap=trap_vector \
          -Wl,--wrap=trap_vector_enclave \
	  -Wl,--wrap=poweroff \
	  -Wl,--wrap=disabled_hart_mask \
          -Wl,--wrap=hart_mask \
          -Wl,--wrap=sanctum_sm_hash \
          -Wl,--wrap=sanctum_sm_signature \
          -Wl,--wrap=sanctum_sm_public_key \
          -Wl,--wrap=sanctum_sm_secret_key \
          -Wl,--wrap=sanctum_dev_public_key \
          -Wl,--wrap=copy1_to_sm \
          -Wl,--wrap=copy_word_to_sm \
          -Wl,--wrap=copy_block_to_sm \
          -Wl,--wrap=copy1_from_sm \
          -Wl,--wrap=copy_word_from_sm \
          -Wl,--wrap=copy_block_from_sm"
         )

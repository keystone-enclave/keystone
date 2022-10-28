cmake_minimum_required(VERSION 3.10)
project(keystone_test C)

SET(SM_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/..)
SET(SM_SRC ${SM_ROOT}/src)
SET(SM_TESTS ${SM_ROOT}/tests)
SET(OPENSBI_SRC ${SM_ROOT}/opensbi)

if (RISCV32)
  SET(CMAKE_C_COMPILER riscv32-unknown-linux-gnu-gcc)
  SET(CROSS_COMPILE riscv32-unknown-elf-)
  SET(QEMU qemu-riscv32)
  SET(LIBCMOCKA ../cmocka/libcmocka-static-32.a)
else ()
  SET(CMAKE_C_COMPILER riscv64-unknown-linux-gnu-gcc)
  SET(CROSS_COMPILE riscv64-unknown-elf-)
  SET(QEMU qemu-riscv64)
  SET(LIBCMOCKA ../cmocka/libcmocka-static.a)
endif()

SET(CMAKE_C_FLAGS "-static")

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ./cmocka/
    ${OPENSBI_SRC}/include
    ${SM_SRC}
)
enable_testing()
SET(CMOCKA_LIBRARY ${LIBCMOCKA})
add_library(cmocka STATIC IMPORTED)
SET_TARGET_PROPERTIES(cmocka PROPERTIES IMPORTED_LOCATION ${CMOCKA_LIBRARY})

add_library(opensbi STATIC IMPORTED)
SET_TARGET_PROPERTIES(opensbi PROPERTIES IMPORTED_LOCATION ${OPENSBI_SRC}/build/lib/libsbi.a)

add_custom_target(opensbi-build
    COMMAND make -C ${OPENSBI_SRC} CROSS_COMPILE=${CROSS_COMPILE} 
      PLATFORM_RISCV_ISA=rv64imafd PLATFORM_RISCV_ABI=lp64d
)
add_dependencies(opensbi opensbi-build)

### skip these symbols
set(MOCK_SYMBOLS
    "-Wl,--wrap=sbi_printf \
     -Wl,--wrap=sbi_hart_hang \
     -Wl,--wrap=spin_lock \
     -Wl,--wrap=spin_unlock \
     -Wl,--wrap=spin_trylock \
     -Wl,--wrap=send_and_sync_pmp_ipi \
     -Wl,--wrap=sbi_pmp_ipi_local_update \
     -Wl,--wrap=sbi_memset \
     -Wl,--wrap=sbi_memcpy \
     -Wl,--wrap=trap_vector_enclave \
     -Wl,--wrap=_trap_handler \
     -Wl,--wrap=ecall_keystone_enclave \
     -Wl,--wrap=sbi_ecall_register_extension \
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
     -Wl,--wrap=copy_block_from_sm \
     "
)

set(MOCK_SOURCE_FILES
    mock/opensbi.c mock/ipi.c mock/mprv.c mock/secure_boot.c)

### test pmp ###
add_executable(test_pmp test_pmp.c ${MOCK_SOURCE_FILES})
target_link_libraries(test_pmp cmocka opensbi)
add_test(test_pmp
	${QEMU} ${CMAKE_CURRENT_BINARY_DIR}/test_pmp)
set_target_properties(test_pmp
	PROPERTIES
	COMPILE_FLAGS -DTARGET_PLATFORM_HEADER=\\"${SM_SRC}\/platform\/generic\/platform.h\\"
	LINK_FLAGS ${MOCK_SYMBOLS}
	)

### test enclave ###
add_executable(test_enclave
	test_enclave.c
	${SM_SRC}/platform/generic/platform.c
	${SM_SRC}/sha3/sha3.c
	${SM_SRC}/ed25519/ge.c
	${SM_SRC}/ed25519/fe.c
	${SM_SRC}/ed25519/sc.c
	${SM_SRC}/ed25519/sign.c
	${SM_SRC}/ed25519/keypair.c
	${SM_SRC}/hkdf_sha3_512/hkdf_sha3_512.c
	${SM_SRC}/hmac_sha3/hmac_sha3.c
	${SM_SRC}/pmp.c
	${SM_SRC}/attest.c
	${SM_SRC}/cpu.c
	${SM_SRC}/crypto.c
	${SM_SRC}/thread.c
	${SM_SRC}/sm.c
	${MOCK_SOURCE_FILES}
	)
target_link_libraries(test_enclave cmocka)
add_test(test_enclave
	${QEMU} ${CMAKE_CURRENT_BINARY_DIR}/test_enclave)
set_target_properties(test_enclave
	PROPERTIES
	COMPILE_FLAGS -DTARGET_PLATFORM_HEADER=\\"${SM_SRC}\/platform\/generic\/platform.h\\"
	LINK_FLAGS ${MOCK_SYMBOLS}
)

cmake_minimum_required(VERSION 3.12)
project(keystone_test C)

SET(CMAKE_C_COMPILER riscv64-unknown-linux-gnu-gcc)
SET(CMAKE_C_FLAGS -static)

include_directories(
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}
    ./cmocka/
    ../../machine
    ../../
    ../../build
)
enable_testing()
SET(CMOCKA_LIBRARY ../cmocka/libcmocka-static.a)
add_library(cmocka STATIC IMPORTED)
SET_TARGET_PROPERTIES(cmocka PROPERTIES IMPORTED_LOCATION ${CMOCKA_LIBRARY})

add_executable(pmp_test pmp_test.c mock/machine.c)
target_link_libraries(pmp_test cmocka)

add_test(pmp_test
        qemu-riscv64 ${CMAKE_CURRENT_BINARY_DIR}/pmp_test)

set_target_properties(pmp_test
        PROPERTIES
        LINK_FLAGS  "-Wl,--wrap=printm -Wl,--wrap=poweroff -Wl,--wrap=disabled_hart_mask -Wl,--wrap=hart_mask"
)

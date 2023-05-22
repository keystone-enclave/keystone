cmake_minimum_required(VERSION 3.2)
project(keystone_bootrom C ASM)

set(CMAKE_C_COMPILER "riscv${BITS}-unknown-elf-gcc")

# This disables use of -rdynamic, which is unrecognized by the compiler we're
# using.
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

# Consider naming .elf for consistency with previous Makefile. 
#set(keystone_bootrom_exe "${PROJECT_NAME}.elf")

add_executable(${PROJECT_NAME})

target_compile_options(
  ${PROJECT_NAME} PRIVATE
  -march=${ISA}
  -mcmodel=medany
  -mabi=${ABI}
  -fno-common
  -std=gnu11
  -fPIC
  -O2
  -Wall
  -w # Disable warnings for now
)

target_link_options(
  ${PROJECT_NAME} PRIVATE
  -nostdlib
  -nostartfiles
  -static
  "SHELL:-T bootloader.lds"
)

target_include_directories(
  ${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_link_directories(
  ${PROJECT_NAME} PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)

target_sources(
  ${PROJECT_NAME} PRIVATE
  bootloader.S
  bootloader.c
  sha3/sha3.c
  ed25519/fe.c
  ed25519/ge.c
  ed25519/keypair.c
  ed25519/sc.c
  ed25519/sign.c
  ed25519/verify.c
)

set(OBJCOPY "riscv${BITS}-unknown-elf-objcopy")

add_custom_target("${PROJECT_NAME}-objcopy"
  DEPENDS ${bootrom_wrkdir} ${PROJECT_NAME}
  COMMAND ${OBJCOPY} --output-target=binary --only-section=.text $<TARGET_FILE:${PROJECT_NAME}> ${bootrom_wrkdir}/bootrom.bin
  COMMENT "Creating bootrom.bin from bootrom.elf"
)
  

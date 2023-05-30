cmake_minimum_required(VERSION 3.2)
project(keystone_bootrom C ASM)

#include(CheckSymbolExists)
#include(CheckSourceCompiles)

# We don't want the flags CMake includes by default. In particular, we need to
# remove -rdynamic, which is unsupported by the compiler we're using.
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

set(bootrom_elf bootrom.elf)

add_executable(${bootrom_elf})

target_compile_options(
  ${bootrom_elf}
  PRIVATE -march=${ISA}
          -mcmodel=medany
          -mabi=${ABI}
          -fno-common
          -std=gnu11
          -fPIC
          -O2
          -Wall
	  -w # Disable warnings for now
	)

target_link_options(${bootrom_elf} PRIVATE -nostdlib -nostartfiles -static
                    "SHELL:-T bootloader.lds")

target_include_directories(${bootrom_elf}
                           PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_directories(${bootrom_elf} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(
  ${bootrom_elf}
  PRIVATE bootloader.S
          bootloader.c
          sha3/sha3.c
          ed25519/fe.c
          ed25519/ge.c
          ed25519/keypair.c
          ed25519/sc.c
          ed25519/sign.c
          ed25519/verify.c)

create_directory(${bootrom_wrkdir})

set(bootrom_bin ${bootrom_wrkdir}/bootrom.bin)

add_custom_command(
  DEPENDS ${bootrom_wrkdir} ${bootrom_elf}
  OUTPUT ${bootrom_bin}
  COMMAND ${OBJCOPY} --output-target=binary --only-section=.text
  $<TARGET_FILE:${bootrom_elf}> ${bootrom_bin}
  COMMENT "Creating bootrom.bin from bootrom.elf")

add_custom_target(${PROJECT_NAME} ALL DEPENDS ${bootrom_bin})

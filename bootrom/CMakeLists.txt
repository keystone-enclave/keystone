cmake_minimum_required(VERSION 3.2)
project(bootrom C ASM)

set(CMAKE_C_COMPILER "riscv${BITS}-unknown-elf-gcc")

# This disables use of -rdynamic, which is unrecognized by the compiler we're
# using.
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS "")

#set(MARCH rv64imafdc)
#set(MABI lp64)
set(OBJCOPY "riscv${BITS}-unknown-elf-objcopy")

add_executable(keystone_bootrom)

target_compile_options(
  keystone_bootrom PRIVATE
  -march=${ISA} 
  -mcmodel=medany
  -mabi=${ABI}
  -fno-common
  -std=gnu11 
  -fPIC
  -O2
  -Wall
  -w  # disable warnings for now
)

target_link_options(
  keystone_bootrom PRIVATE
  -nostdlib
  -nostartfiles
  -static
  "SHELL:-T bootloader.lds"  
)

target_include_directories(keystone_bootrom PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
  ./
)
target_link_directories(keystone_bootrom PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(
  keystone_bootrom PRIVATE
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

add_custom_target("bootrom-objcopy" DEPENDS ${bootrom_wrkdir}
  COMMAND ${OBJCOPY} --output-target=binary --only-section=.text keystone_bootrom ${bootrom_wrkdir}/bootrom.bin
)






option(RISCV32 "Build in 32-bit mode" OFF)
if(RISCV32)
  set(BITS 32)
  set(ABI ilp32d)
else()
  set(BITS 64)
  set(ABI lp64d)
endif()

set(ISA "rv${BITS}imafdc")
set(cross_compile riscv${BITS}-unknown-linux-gnu-)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR riscv${BITS})
set(CMAKE_FIND_ROOT_PATH "${CMAKE_CURRENT_LIST_DIR}/riscv${BITS}")
set(CMAKE_C_COMPILER riscv${BITS}-unknown-elf-gcc)
set(CMAKE_CXX_COMPILER riscv${BITS}-unknown-elf-g++)
set(OBJCOPY "riscv${BITS}-unknown-elf-objcopy")

project(keystone-host CXX)

set(CFLAGS      "-Wall -Werror")
set(LDFLAGS     "-static")

set(SOURCE_FILES
  elf.c
  elf32.c
  elf64.c
  hash_util.cpp
  ElfFile.cpp
  KeystoneDevice.cpp
  Enclave.cpp
  Memory.cpp
  PhysicalEnclaveMemory.cpp
  SimulatedEnclaveMemory.cpp
  )

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/host)

set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS} ${CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

include_directories(${INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${COMMON_SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${out_dir}/lib)
install(DIRECTORY ${INCLUDE_DIRS} DESTINATION ${out_dir}/include)

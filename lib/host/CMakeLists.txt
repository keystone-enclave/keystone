project(keystone-host CXX)

set(CFLAGS      "-Wall -Werror")
set(LDFLAGS     "-static")

set(SOURCE_FILES
        src/elf.c
        src/elf32.c
        src/elf64.c
        src/hash_util.cpp
        src/ElfFile.cpp
        src/KeystoneDevice.cpp
        src/Enclave.cpp
        src/Memory.cpp
        src/PhysicalEnclaveMemory.cpp
        src/SimulatedEnclaveMemory.cpp
    )

set(INCLUDE_DIRS include)

set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS} ${CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

include_directories(${INCLUDE_DIRS} ${COMMON_INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${COMMON_SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${out_dir})

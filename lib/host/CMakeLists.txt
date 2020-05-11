project(keystone-host CXX)

set(CFLAGS      "-Wall -Werror")
set(LDFLAGS     "-static")

set(SOURCE_FILES 
        src/elf.cpp
        src/elf32.cpp
        src/elf64.cpp
        src/elffile.cpp
        src/keystone_device.cpp
        src/keystone.cpp
        src/memory.cpp
        src/sha3.cpp
    )
    
set(INCLUDE_DIRS include)

set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS} ${CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

include_directories(${INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME} DESTINATION ${out_dir})


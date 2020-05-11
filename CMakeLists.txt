cmake_minimum_required(VERSION 3.10)

#-------------------------------------------------------------------------------
# BASIC SETUP
#-------------------------------------------------------------------------------

set(prog_name    keystone-sdk)
set(src_dir      ${CMAKE_CURRENT_LIST_DIR})
set(scripts_dir  ${src_dir}/scripts)

if (OUTPUT_DIR)
    set(out_dir      ${OUTPUT_DIR})
else()
    set(out_dir      ${CMAKE_CURRENT_LIST_DIR})
endif()

#-------------------------------------------------------------------------------
# Program and flags
#-------------------------------------------------------------------------------

include(macros.cmake)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE    "Debug")
else()
    if ((NOT CMAKE_BUILD_TYPE STREQUAL "Debug") AND (NOT CMAKE_BUILD_TYPE STREQUAL "Release"))
    message(FATAL_ERROR "CMAKE_BUILD_TYPE must either be Debug or Release instead of ${CMAKE_BUILD_TYPE}")
    endif()
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG=1)
endif()

execute_process(
    COMMAND which riscv64-unknown-linux-gnu-gcc
    OUTPUT_VARIABLE CROSSCOMPILE
    RESULT_VARIABLE ERROR)
    
if (NOT "${ERROR}" STREQUAL 0)
    message(FATAL_ERROR "RISCV Toochain is not found")
endif()

string(STRIP ${CROSSCOMPILE} CROSSCOMPILE)
string(REPLACE "gcc" "" CROSSCOMPILE ${CROSSCOMPILE})

message(STATUS "Tagret tripplet: ${CROSSCOMPILE}")

set(CC              ${CROSSCOMPILE}gcc)
set(CXX             ${CROSSCOMPILE}g++)
set(LD              ${CROSSCOMPILE}ld)
set(AR              ${CROSSCOMPILE}ar)
set(OBJCOPY         ${CROSSCOMPILE}objcopy)
set(OBJDUMP         ${CROSSCOMPILE}objdump)
set(CFLAGS          "-Wall -Werror")

global_set(CMAKE_C_COMPILER        ${CC}${EXT})
global_set(CMAKE_CXX_COMPILER      ${CXX}${EXT})
global_set(CMAKE_LINKER            ${LD}${EXT})
global_set(CMAKE_AR                ${AR}${EXT})
global_set(CMAKE_OBJCOPY           ${OBJCOPY}${EXT})
global_set(CMAKE_OBJDUMP           ${OBJDUMP}${EXT})
global_set(CMAKE_C_FLAGS           ${CMAKE_C_FLAGS} ${CFLAGS})

check_compiler(${CMAKE_C_COMPILER})
check_compiler(${CMAKE_CXX_COMPILER})

global_set(CMAKE_C_COMPILER_WORKS      1)
global_set(CMAKE_CXX_COMPILER_WORKS    1)

global_set(CMAKE_SYSTEM_NAME    "Linux")

################################################################################
# BUILD PROJECTS
################################################################################

add_subdirectory(lib)








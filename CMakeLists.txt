cmake_minimum_required(VERSION 3.10)
enable_language(C CXX ASM)
#-------------------------------------------------------------------------------
# BASIC SETUP
#-------------------------------------------------------------------------------

if(RISCV32)
  message(STATUS "riscv32")
  set(BITS 32)
else()
  message(STATUS "riscv64")
  set(BITS 64)
endif()

set(prog_name    keystone-sdk)
set(src_dir      ${CMAKE_CURRENT_LIST_DIR})
set(scripts_dir  ${src_dir}/scripts)
set(cross_compile riscv${BITS}-unknown-linux-gnu-)


if (OUTPUT_DIR)
  if (NOT IS_ABSOLUTE ${OUTPUT_DIR})
    message(FATAL_ERROR "OUTPUT_DIR needs to be absolute path")
  endif()
  get_filename_component(OUTPUT_DIR ${OUTPUT_DIR} ABSOLUTE)
  set(out_dir    ${OUTPUT_DIR})
else()
  set(out_dir    ${CMAKE_BINARY_DIR})
endif()

if (${out_dir} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "OUTPUT_DIR must be different from the source path")
endif()

message(" *** Install path: ${out_dir}")

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
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -g)
endif()

execute_process(
    COMMAND which ${cross_compile}gcc
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
global_set(CMAKE_ASM_COMPILER        ${CC}${EXT})
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
include_directories(include)
add_subdirectory(src)

################################################################################
# Auto Formatting
################################################################################
file(GLOB_RECURSE
  CHECK_CXX_SOURCE_FILES
  src/*.cpp include/*.hpp
  example/*.cpp tests/*.cpp
  example/*.hpp tests/*.hpp
)

# remove external cpp sources from cpplint checking
list(FILTER CHECK_CXX_SOURCE_FILES EXCLUDE REGEX ".*/json11.cpp$")
list(FILTER CHECK_CXX_SOURCE_FILES EXCLUDE REGEX ".*/json11.h$")

file(GLOB_RECURSE
  CHECK_C_SOURCE_FILES
  src/*.c include/*.h
  example/*.c tests/*.c
  example/*.h tests/*.h
)

find_program(CLANG_FORMAT "clang-format")
find_program(CPPLINT "cpplint")

if(CLANG_FORMAT AND CPPLINT)
  add_custom_target(
    format
    COMMAND
      ${CLANG_FORMAT}
      -i
      -style=file
      ${CHECK_CXX_SOURCE_FILES} ${CHECK_C_SOURCE_FILES}
    COMMAND
      ${CPPLINT}
      ${CHECK_CXX_SOURCE_FILES}
    COMMENT "Auto-formatting"
  )
endif()

################################################################################
# Uninstall
################################################################################
add_custom_target("uninstall"
  COMMAND
  rm -rf ${out_dir}/lib
  rm -rf ${out_dir}/include)

add_subdirectory(examples EXCLUDE_FROM_ALL)

add_subdirectory(.post-install)

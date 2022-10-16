include(macros.cmake)

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

if (DEFINED ENV{KEYSTONE_SDK_DIR})
  set(KEYSTONE_SDK_DIR $ENV{KEYSTONE_SDK_DIR})
  if (NOT IS_ABSOLUTE ${KEYSTONE_SDK_DIR})
    message(FATAL_ERROR "KEYSTONE_SDK_DIR needs to be absolute path")
  endif()
  get_filename_component(KEYSTONE_SDK_DIR ${KEYSTONE_SDK_DIR} ABSOLUTE)
  set(out_dir    ${KEYSTONE_SDK_DIR})
else()
  message(FATAL_ERROR " * Set KEYSTONE_SDK_DIR to the path you want to install the SDK.\n"
    " * Try `export KEYSTONE_SDK_DIR=<path/to/SDK>`")
  set(out_dir    ${CMAKE_BINARY_DIR})
endif()

if (${out_dir} STREQUAL ${CMAKE_SOURCE_DIR})
  message(FATAL_ERROR "OUTPUT_DIR must be different from the source path")
endif()

message(" *** Install path: ${out_dir}")

#-------------------------------------------------------------------------------
# Program and flags
#-------------------------------------------------------------------------------


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

use_riscv_toolchain(${BITS})
################################################################################
# BUILD PROJECTS
################################################################################
include_directories(include)
add_subdirectory(src)
install(FILES macros.cmake DESTINATION ${out_dir}/cmake/)

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
  rm -rf ${out_dir}/include
  rm -rf ${out_dir}/cmake)

add_subdirectory(examples EXCLUDE_FROM_ALL)
add_subdirectory(tests EXCLUDE_FROM_ALL)

add_subdirectory(.post-install)

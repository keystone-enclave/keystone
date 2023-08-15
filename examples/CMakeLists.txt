cmake_minimum_required(VERSION 3.10)
project(keystone_examples C CXX ASM)

# check if SDK has been built and KEYSTONE_SDK_DIR configuration variable exists
if (NOT DEFINED KEYSTONE_SDK_DIR)
  message(FATAL_ERROR "Please set KEYSTONE_SDK_DIR configuration variable to an installed SDK path")
endif()

set(ENV{KEYSTONE_SDK_DIR} ${KEYSTONE_SDK_DIR})

if (DEFINED PATH)
  set(ENV{PATH} ${PATH})
endif()

include(${KEYSTONE_SDK_DIR}/cmake/macros.cmake)
include(ExternalProject)
find_package(Git REQUIRED)

if(RISCV32)
  message(STATUS "riscv32")
  set(BITS 32)
else()
  message(STATUS "riscv64")
  set(BITS 64)
endif()

use_riscv_toolchain(${BITS})

# export include directory
include_directories(AFTER ${KEYSTONE_SDK_DIR}/include)

# set paths to the libraries
set(KEYSTONE_LIB_HOST ${KEYSTONE_SDK_DIR}/lib/libkeystone-host.a)
set(KEYSTONE_LIB_EDGE ${KEYSTONE_SDK_DIR}/lib/libkeystone-edge.a)
set(KEYSTONE_LIB_VERIFIER ${KEYSTONE_SDK_DIR}/lib/libkeystone-verifier.a)
set(KEYSTONE_LIB_EAPP ${KEYSTONE_SDK_DIR}/lib/libkeystone-eapp.a)
set(KEYSTONE_LIB_UTIL ${KEYSTONE_SDK_DIR}/lib/libkeystone-util.a)

# create a phony target "examples"
add_custom_target("examples")

# add all examples below
add_subdirectory(hello)
add_subdirectory(hello-native)
add_subdirectory(attestation)
add_subdirectory(tests)
add_subdirectory(devshare)
add_subdirectory(ports)
add_subdirectory(maps)
add_subdirectory(vta)

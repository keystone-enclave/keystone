cmake_minimum_required(VERSION 3.10)

include(../macros.cmake)

project(keystone-sdk-test C CXX)

set(SOURCES
  keystone_test.cpp)

SET(CTEST_OUTPUT_ON_FAILURE ON)

set(CMAKE_C_COMPILER  gcc)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_CXX_COMPILER  g++)
set(CMAKE_C_FLAGS "-Wall")

find_package(GTest)

file(GLOB_RECURSE
  HOST_LIB_SOURCES
  ../src/host/*)
file(GLOB
  HOST_LIB_INCLUDE
  ../include/host)
file(GLOB_RECURSE
  COMMON_SOURCES
  ../src/common/*)
file(GLOB
  COMMON_INCLUDE
  ../include/common)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include ${HOST_LIB_INCLUDE} ${COMMON_INCLUDE})
add_executable(TestKeystone
  ${SOURCES}
  ${HOST_LIB_SOURCES} ${COMMON_SOURCES})

message(STATUS ${GTEST_FOUND})
target_link_libraries(TestKeystone ${GTEST_LIBRARIES} pthread)

add_test(NAME TestKeystone
  COMMAND ./TestKeystone)

add_custom_target(check DEPENDS binaries
  COMMAND env CTEST_OUTPUT_ON_FAILURE=1 GTEST_COLOR=1
  ${CMAKE_CTEST_COMMAND}
  DEPENDS TestKeystone)

enable_testing()

add_custom_target(binaries ALL
  COMMAND
  env CMAKE_SOURCE_DIR=${CMAKE_CURRENT_SOURCE_DIR}/ CMAKE_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}/
  ${CMAKE_CURRENT_SOURCE_DIR}/scripts/setup_binary.sh
)

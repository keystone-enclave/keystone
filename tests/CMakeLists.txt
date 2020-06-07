cmake_minimum_required(VERSION 3.10)

project(keystone-sdk-test C CXX)

set(SOURCES
  keystone_test.cpp)

SET(CTEST_OUTPUT_ON_FAILURE ON)

set(CMAKE_C_COMPILER  gcc)
set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_CXX_COMPILER  g++)
set(CMAKE_C_FLAGS "-Wall")

find_package(GTest REQUIRED)

file(GLOB_RECURSE
  HOST_LIB_SOURCES
  ../lib/host/src/*)
file(GLOB
  HOST_LIB_INCLUDE
  ../lib/host/include)
file(GLOB_RECURSE
  COMMON_SOURCES
  ../common/src/*)
file(GLOB
  COMMON_INCLUDE
  ../common/include)

include_directories(${PROJECT_NAME} PUBLIC ${HOST_LIB_INCLUDE} ${COMMON_INCLUDE})
add_executable(TestKeystone
  ${SOURCES}
  ${HOST_LIB_SOURCES} ${COMMON_SOURCES})

message(STATUS ${GTEST_FOUND})
target_link_libraries(TestKeystone ${GTEST_LIBRARIES})

add_test(NAME TestKeystone
  COMMAND ./TestKeystone)

add_custom_target(check
  COMMAND env CTEST_OUTPUT_ON_FAILURE=1 GTEST_COLOR=1
  ${CMAKE_CTEST_COMMAND}
  DEPENDS TestKeystone)

enable_testing()

add_subdirectory(test_binary)

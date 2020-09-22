cmake_minimum_required(VERSION 3.0)
project(eyrie_test)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmocka/cmake/Modules")

set(WITH_EXAMPLES false)
add_subdirectory(cmocka)

include(AddCMockaTest)
enable_testing()

add_cmocka_test(test_string SOURCES string.c COMPILE_OPTIONS -I${CMAKE_BINARY_DIR}/cmocka/include LINK_LIBRARIES cmocka)
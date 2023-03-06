cmake_minimum_required(VERSION 3.13)
project(keystone-runtime C ASM)

macro(rt_option name description value)
    option(${name} ${description} ${value})
    if(${name})
        add_compile_options(-DUSE_${name})
        add_custom_target(${name}_options_log
                            COMMAND echo -n "${name} " >> ${CMAKE_BINARY_DIR}/.options_log)
        add_dependencies(options_log ${name}_options_log)

        message(STATUS "Enabling option ${name}")
    endif()
endmacro()

###########################
## Set up global options ##
###########################

if(NOT DEFINED ENV{KEYSTONE_SDK_DIR})
    message(FATAL_ERROR "Set KEYSTONE_SDK_DIR in environment")
endif()

add_custom_target(options_log
        COMMAND touch ${CMAKE_BINARY_DIR}/.options_log)

# Memory management options
rt_option(FREEMEM "Use freemem allocator" OFF)
rt_option(PAGING "Enable runtime paging" OFF)
rt_option(PAGE_CRYPTO "Enable page confidentiality" OFF)
rt_option(PAGE_HASH "Enable page integrity" OFF)

# Syscall options
rt_option(LINUX_SYSCALL "Wrap generic Linux syscalls" OFF)
rt_option(IO_SYSCALL "Wrap Linux IO syscalls" OFF)
rt_option(NET_SYSCALL "Wrap Linux net syscalls" OFF)

# System options
rt_option(ENV_SETUP "Set up stack environments like glibc expects" OFF)

# Debugging options
rt_option(INTERNAL_STRACE "Debug syscalls" OFF)
rt_option(DEBUG "Enable debugging" OFF)

include_directories($ENV{KEYSTONE_SDK_DIR}/include/edge)
include_directories(tmplib)
include_directories(include)

###################
## Build recipes ##
###################

add_compile_options(-Wall -Werror -fPIC -fno-builtin -std=c11 -g)

# Generate all the library targets
add_subdirectory(call)
add_subdirectory(crypto)
add_subdirectory(mm)
add_subdirectory(tmplib)
add_subdirectory(util)

# Generate linker script target
add_library(rt_linkscript OBJECT runtime.ld.S)
target_compile_options(rt_linkscript PRIVATE -P -E -x c)
target_compile_definitions(rt_linkscript PRIVATE __PREPROCESSING__)

set_target_properties(rt_linkscript PROPERTIES PREFIX "")
set_target_properties(rt_linkscript PROPERTIES OUTPUT_NAME runtime.ld)
set_target_properties(rt_linkscript PROPERTIES SUFFIX "")

# Generate final executable
add_subdirectory(sys)

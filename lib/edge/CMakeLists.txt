project(keystone-edge C ASM)

set(CFLAGS      "-Wall -Werror")
set(LDFLAGS     "-static")
set(DEFINES     "-DIO_SYSCALL_WRAPPING")

set(SOURCE_FILES 
        src/edge_call.c
        src/edge_dispatch.c
        src/edge_syscall.c
    )
    
set(INCLUDE_DIRS include)

set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS} ${CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

include_directories(${INCLUDE_DIRS})
add_definitions(${DEFINES})

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME} DESTINATION ${out_dir})


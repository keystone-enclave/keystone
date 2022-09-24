project(keystone-eapp C ASM)

set(CFLAGS      "-Wall -Werror")
set(LDFLAGS     "-static")

set(SOURCE_FILES
  encret.s
  string.c
  syscall.c
  tiny-malloc.c
  )

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/app)

set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS} ${CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

include_directories(${INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${out_dir}/lib)
install(DIRECTORY ${INCLUDE_DIRS} DESTINATION ${out_dir}/include)

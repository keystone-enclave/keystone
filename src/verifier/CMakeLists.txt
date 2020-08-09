project(keystone-verifier C CXX)

set(CFLAGS      "-Wall -Werror")
set(LDFLAGS     "-static")

set(SOURCE_FILES
    json11.cpp
    keys.cpp
    Report.cpp
    ed25519/fe.c
    ed25519/ge.c
    ed25519/keypair.c
    ed25519/sc.c
    ed25519/sign.c
    ed25519/verify.c
    )

set(INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/include/verifier)

set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS} ${CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

include_directories(${INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${SOURCE_FILES} ${COMMON_SOURCE_FILES})

install(TARGETS ${PROJECT_NAME} DESTINATION ${out_dir}/lib)
install(DIRECTORY ${INCLUDE_DIRS} DESTINATION ${out_dir}/include)

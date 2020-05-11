project(keystone-verifier C CXX)

set(CFLAGS      "-Wall -Werror")
set(LDFLAGS     "-static")

set(SOURCE_FILES 
    json11.cpp
    keys.cpp
    report.cpp
    sha3/sha3.c
    ed25519/fe.c
    ed25519/ge.c
    ed25519/keypair.c
    ed25519/sc.c
    ed25519/sign.c
    ed25519/verify.c
    )
    
set(INCLUDE_DIRS 
        include 
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
 
set(CMAKE_C_FLAGS          "${CMAKE_C_FLAGS} ${CFLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${LDFLAGS}")

include_directories(${INCLUDE_DIRS})

add_library(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC ${INCLUDE_DIRS})

install(TARGETS ${PROJECT_NAME} DESTINATION ${out_dir})


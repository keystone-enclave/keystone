
set(SYS_SOURCES entry.S boot.c env.c interrupt.c)
add_executable(eyrie-build EXCLUDE_FROM_ALL ${SYS_SOURCES})

# The ordering of these libraries is important, make sure that any symbols which may be
# required by one library are defined by the time that it is added to this list

target_link_libraries(eyrie-build
        rt_call rt_mm rt_crypto rt_tmplib rt_util
        gcc $ENV{KEYSTONE_SDK_DIR}/lib/libkeystone-edge.a)
target_link_options(eyrie-build PRIVATE -static -nostdlib -T $<TARGET_OBJECTS:rt_linkscript>)
add_dependencies(eyrie-build rt_linkscript)

add_custom_target(eyrie-rt ALL
        DEPENDS options_log eyrie-build
        COMMAND ${CMAKE_OBJCOPY} -I binary
            --add-section .options_log=${CMAKE_BINARY_DIR}/.options_log
            --set-section-flags .options_log=noload,readonly
            ${CMAKE_CURRENT_BINARY_DIR}/eyrie-build)

add_custom_command(TARGET eyrie-rt POST_BUILD
        COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/eyrie-build ${CMAKE_SOURCE_DIR}/eyrie-rt || true
        COMMAND cp ${CMAKE_BINARY_DIR}/.options_log ${CMAKE_SOURCE_DIR}/.options_log || true)

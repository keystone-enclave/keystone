
set(LOADER_SOURCES loader.S loader-binary.c)
set(LOADER_LINK_SCRIPT loader.lds)

set(MANUAL_COMPILE_CFLAGS -Wall -Werror -fno-builtin -nostdlib -g -mcmodel=medany -static -DLOADER_BIN -DUSE_FREEMEM -I../include)
set(MANUAL_COMPILED ${CMAKE_CURRENT_BINARY_DIR}/loader.S.o ${CMAKE_CURRENT_BINARY_DIR}/loader-binary.o)
add_custom_target(loader-manual-compile
        COMMAND ${CMAKE_C_COMPILER} ${MANUAL_COMPILE_CFLAGS} -c loader.S -o loader.S.o
        COMMAND ${CMAKE_C_COMPILER} ${MANUAL_COMPILE_CFLAGS} -c loader-binary.c -o loader-binary.o
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

execute_process(COMMAND cmake -E touch ${CMAKE_CURRENT_BINARY_DIR}/dummy.c)
add_executable(loader dummy.c)
target_link_libraries(loader rt_call ld_mm rt_util rt_loader)
target_compile_options(loader PRIVATE -static -nostdlib -mcmodel=medany)
target_link_options(loader PRIVATE -static -nostdlib -g -fPIC -T ${LOADER_LINK_SCRIPT} ${MANUAL_COMPILED})
add_dependencies(loader loader-manual-compile)

add_custom_target(loader.bin ALL
        DEPENDS loader
        COMMAND ${CMAKE_OBJCOPY} -O binary --only-section .text
                ${CMAKE_CURRENT_BINARY_DIR}/loader
                ${CMAKE_SOURCE_DIR}/loader.bin
)

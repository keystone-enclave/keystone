
# TODO: fix. The below produces broken loader.

set(LOADER_SOURCES loader.S loader.c elf.c elf32.c elf64.c mem.c mm.c printf.c sbi.c string.c vm.c)
set(LOADER_LINK_SCRIPT loader.lds)

add_executable(loader EXCLUDE_FROM_ALL ${LOADER_SOURCES})
target_include_directories(loader BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_options(loader PRIVATE -Wall -Werror -fno-builtin -nostdlib -g -mcmodel=medany -static)
target_link_libraries(loader gcc)
target_link_options(loader PRIVATE -static -nostdlib -g -fPIC -T ${LOADER_LINK_SCRIPT})

add_custom_target(loader.bin ALL
        DEPENDS loader
        COMMAND ${CMAKE_OBJCOPY} -O binary --only-section .text
                ${CMAKE_CURRENT_BINARY_DIR}/loader
                ${CMAKE_SOURCE_DIR}/loader.bin
)

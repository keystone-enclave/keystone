
# TODO: fix. Both of the below produce broken loader. Calling make from here infinite loops.

set(LOADER_SOURCES loader.S loader-binary.c mm.c)
set(LOADER_LINK_SCRIPT loader.lds)

# add_executable(loader EXCLUDE_FROM_ALL ${LOADER_SOURCES})
# target_link_libraries(loader
#         rt_call ld_mm rt_util rt_loader
# )
# target_compile_options(loader PRIVATE -static -nostdlib -mcmodel=medany -DLOADER_BIN)
# target_link_options(loader PRIVATE -static -nostdlib -g -fPIC -T ${LOADER_LINK_SCRIPT})

set(LDBIN_LIBS ../loader/librt_loader.a ../util/librt_util.a ../mm/libld_mm.a ../call/librt_call.a)
set(LDBIN_CFLAGS -Wall -Werror -fno-builtin -nostdlib -g -mcmodel=medany -static -DLOADER_BIN -DUSE_FREEMEM -I../include)
set(LDBIN_LDFLAGS -fPIC -nostdlib -g -static -T ${LOADER_LINK_SCRIPT})
set(LDBIN_CC riscv64-buildroot-linux-gnu-gcc)
add_custom_target(loader ALL
        DEPENDS rt_call ld_mm rt_util rt_loader
        COMMAND cp ../mm/mm.c . && ${LDBIN_CC} ${LDBIN_CFLAGS} ${LDBIN_LDFLAGS} ${LOADER_SOURCES} -o loader ${LDBIN_LIBS}
)

add_custom_target(loader.bin ALL
        DEPENDS loader
        COMMAND ${CMAKE_OBJCOPY} -O binary --only-section .text
                ${CMAKE_CURRENT_BINARY_DIR}/loader
                ${CMAKE_SOURCE_DIR}/loader.bin
)

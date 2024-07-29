
set(LOADER_SOURCES loader.S loader-binary.c)
set(LOADER_LINK_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/loader.lds)

add_executable(loader ${LOADER_SOURCES})
target_link_libraries(loader rt_call ld_mm rt_util rt_tmplib rt_loader gcc)
target_link_options(loader PRIVATE -static -nostdlib -T ${LOADER_LINK_SCRIPT})

add_custom_target(loader.bin ALL
        DEPENDS loader
        COMMAND ${CMAKE_OBJCOPY} -O binary --only-section .text
                ${CMAKE_CURRENT_BINARY_DIR}/loader
                ${CMAKE_SOURCE_DIR}/loader.bin)

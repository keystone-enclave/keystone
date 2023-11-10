
set(MM_SOURCES vm.c page_swap.c)

if(FREEMEM)
    list(APPEND MM_SOURCES mm.c freemem.c)
endif()

if(PAGING)
    list(APPEND MM_SOURCES paging.c)
endif()

add_library(rt_mm ${MM_SOURCES})

# TODO: fix below. Doing mm.c with -fPIC hangs map_page's call to __walk_create
set(LD_MM_SOURCES vm.c freemem_ld.c)
add_library(ld_mm ${LD_MM_SOURCES})
target_compile_options(ld_mm PUBLIC -DLOADER_BIN -DUSE_FREEMEM)
target_link_libraries(ld_mm ${CMAKE_CURRENT_BINARY_DIR}/mm.o)
add_dependencies(ld_mm mmc-manual-compile)

set(MANUAL_COMPILE_CFLAGS -Wall -Werror -fno-builtin -nostdlib -g -mcmodel=medany -static -DLOADER_BIN -DUSE_FREEMEM -I../include)
add_custom_target(mmc-manual-compile
        COMMAND ${CMAKE_C_COMPILER} ${MANUAL_COMPILE_CFLAGS} -c mm.c -o mm.o
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

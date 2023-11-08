
set(MM_SOURCES vm.c page_swap.c)

if(FREEMEM)
    list(APPEND MM_SOURCES mm.c freemem.c)
endif()

if(PAGING)
    list(APPEND MM_SOURCES paging.c)
endif()

add_library(rt_mm ${MM_SOURCES})

# TODO: fix below. Doing mm.c here hangs map_page's call to __walk_create
set(LD_MM_SOURCES vm.c mm.c freemem_ld.c)
add_library(ld_mm ${LD_MM_SOURCES})
target_compile_options(ld_mm PUBLIC -DLOADER_BIN -DUSE_FREEMEM)


set(MM_SOURCES vm.c page_swap.c mm.c freemem.c)

if(PAGING)
    list(APPEND MM_SOURCES paging.c)
endif()

add_library(rt_mm ${MM_SOURCES})

set(LD_MM_SOURCES vm.c freemem_ld.c mm.c)
add_library(ld_mm ${LD_MM_SOURCES})
target_compile_options(ld_mm PUBLIC -DLOADER_BIN)

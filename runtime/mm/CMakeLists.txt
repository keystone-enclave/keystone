
set(MM_SOURCES vm.c page_swap.c)

if(FREEMEM)
    list(APPEND MM_SOURCES mm.c freemem.c)
endif()

if(PAGING)
    list(APPEND MM_SOURCES paging.c)
endif()

add_library(rt_mm ${MM_SOURCES})

set(LD_MM_SOURCES vm.c freemem_ld.c mm.c)
add_library(ld_mm ${LD_MM_SOURCES})
# All are required. medany is PIC. Adding -fPIC breaks.
target_compile_options(ld_mm PRIVATE -static -mcmodel=medany)
target_compile_options(ld_mm PUBLIC -DLOADER_BIN -DUSE_FREEMEM)

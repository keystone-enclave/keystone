
set(MM_SOURCES vm.c page_swap.c)

if(FREEMEM)
    list(APPEND MM_SOURCES mm.c freemem.c)
endif()

if(PAGING)
    list(APPEND MM_SOURCES paging.c)
endif()

add_library(rt_mm ${MM_SOURCES})

set(MM_SOURCES vm.c page_swap.c mm.c freemem.c)

if(PAGING)
    list(APPEND MM_SOURCES paging.c)
endif()

add_library(rt_mm ${MM_SOURCES})

set(LD_MM_SOURCES vm.c freemem_ld.c mm.c)
add_library(ld_mm ${LD_MM_SOURCES})
target_compile_options(ld_mm PUBLIC -DLOADER_BIN)

# Filter out any externally defined use flags
get_target_property(LD_MM_CFLAGS ld_mm COMPILE_OPTIONS)
list(FILTER LD_MM_CFLAGS EXCLUDE REGEX "-DUSE_.*")
set_target_properties(ld_mm PROPERTIES COMPILE_OPTIONS "${LD_MM_CFLAGS}")

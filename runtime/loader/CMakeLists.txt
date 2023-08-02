
set(LOADER_SOURCES elf.c elf32.c elf64.c)

if(FREEMEM)
    list(APPEND LOADER_SOURCES loader.c)
endif()

add_library(rt_loader ${LOADER_SOURCES})

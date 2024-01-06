
set(LOADER_SOURCES elf.c elf32.c elf64.c loader.c)
add_library(rt_loader ${LOADER_SOURCES})

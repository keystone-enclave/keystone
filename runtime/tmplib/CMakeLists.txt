
set(TMPLIB_SOURCES uaccess.S)

add_library(rt_tmplib ${TMPLIB_SOURCES})
target_compile_options(rt_tmplib PRIVATE -include partial_linkage.h)
target_compile_definitions(rt_tmplib PRIVATE __ASSEMBLY__)
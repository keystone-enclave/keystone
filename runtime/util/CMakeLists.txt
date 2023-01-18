
set(UTIL_SOURCES printf.c rt_util.c spinlock.S string.c)
add_library(rt_util ${UTIL_SOURCES})

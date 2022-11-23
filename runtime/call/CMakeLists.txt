
set(CALL_SOURCES sbi.c syscall.c)

if(LINUX_SYSCALL)
    list(APPEND CALL_SOURCES linux_wrap.c)
endif()

if(IO_SYSCALL)
    list(APPEND CALL_SOURCES io_wrap.c)
endif()

if(NET_SYSCALL)
    list(APPEND CALL_SOURCES net_wrap.c)
endif()

add_library(rt_call STATIC ${CALL_SOURCES})

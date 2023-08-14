
set(DRIVER_SOURCES drivers.c serial.c cma.c vta.c)
add_library(rt_drivers OBJECT ${DRIVER_SOURCES})

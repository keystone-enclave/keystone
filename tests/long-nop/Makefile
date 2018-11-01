APP = long-nop
APP_A_SRCS = long-nop.s add_long.s
EXTRA_CLEAN = add_long.s

include ../app.mk
add_long.s: generate_func.sh nop.s func_base.s
	./generate_func.sh 4097 add_long.s


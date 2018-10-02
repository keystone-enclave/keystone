CC = riscv64-unknown-linux-gnu-gcc
OBJCOPY = riscv64-unknown-linux-gnu-objcopy
XXD = xxd

all: add_long.eapp_riscv

add_long.eapp_riscv:
	./generate_func.sh 4096 func_long.s
	$(CC) -I../../../include -pie -c func_long.s
	$(OBJCOPY) -O binary --only-section=.text func_long.o add_long.o
	$(XXD) -i -a add_long.o > add_long.h
	mv add_long.o add_long.eapp_riscv
clean:
	rm -f *.o add_long.h *.eapp_riscv

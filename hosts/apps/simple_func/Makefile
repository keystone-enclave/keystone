CC = riscv64-unknown-linux-gnu-gcc
OBJCOPY = riscv64-unknown-linux-gnu-objcopy
XXD = xxd

all:

	$(CC) -I../../../include -pie -c func.s
	$(OBJCOPY) -O binary --only-section=.text func.o add.o
	$(XXD) -i -a add.o > add.h

	./generate_func.sh 4096 func_long.s
	$(CC) -I../../../include -pie -c func_long.s
	$(OBJCOPY) -O binary --only-section=.text func_long.o add_long.o
	$(XXD) -i -a add_long.o > add_long.h


clean:
	rm -f func_long.s func.o func_long.o add.o add_long.o add.h add_long.h

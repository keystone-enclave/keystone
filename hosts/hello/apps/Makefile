CC = riscv64-unknown-linux-gnu-gcc
OBJCOPY = riscv64-unknown-linux-gnu-objcopy
XXD = xxd

all: add.eapp_riscv

add.eapp_riscv:
	$(CC) -I../../../include -pie -c func.s
	$(OBJCOPY) -O binary --only-section=.text func.o add.o
	$(XXD) -i -a add.o > add.h
	mv add.o add.eapp_riscv

clean:
	rm -f *.o add.h *.eapp_riscv

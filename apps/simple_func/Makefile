CC = riscv64-unknown-linux-gnu-gcc
OBJCOPY = riscv64-unknown-linux-gnu-objcopy
XXD = xxd
all:
	$(CC) -I../../include -pie -c func.s
	$(OBJCOPY) -O binary --only-section=.text func.o add.o
	$(XXD) -i -a add.o > add.h

clean:
	rm -f func.o add.o add.h

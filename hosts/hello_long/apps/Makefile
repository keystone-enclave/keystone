CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -Wall -Werror -fPIC
LINK = riscv64-unknown-linux-gnu-ld
LINKFLAGS = -static


all: add_long.eapp_riscv

add_long.eapp_riscv: app.lds
	./generate_func.sh 1 func_long.s
	$(CC) -I../../../include -c func_long.s
	$(LINK) $(LINKFLAGS) -o $@ func_long.o -T app.lds
	./parse_elf.sh $@ add_long_entry
clean:
	rm -f *.o add_long.h *.eapp_riscv

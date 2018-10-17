CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -Wall -Werror -fPIC
LINK = riscv64-unknown-linux-gnu-ld
LINKFLAGS = -static
APP_LDS = ../../../scripts/app.lds

APPS = add_long
APP_BINS = $(patsubst %,%.eapp_riscv,$(APPS))

all: $(APP_BINS)

%.o: %.s
	$(CC) -I../../../include -c $<

%.eapp_riscv: %.o $(APP_LDS)
	$(LINK) $(LINKFLAGS) -o $@ $< -T $(APP_LDS)
	../../../scripts/parse_elf.sh $@ $*_entry

add_long.s: generate_func.sh nop.s func_base.s
	./generate_func.sh 4097 add_long.s

clean:
	rm -f *.o *.eapp_riscv $(patsubst %,%_entry.h,$(APPS)) add_long.s

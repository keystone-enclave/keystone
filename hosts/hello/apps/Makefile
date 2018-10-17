CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -Wall -Werror -fPIC
LINK = riscv64-unknown-linux-gnu-ld
LINKFLAGS = -static
APP_LDS = ../../../scripts/app.lds

APPS = add
APP_BINS = $(patsubst %,%.eapp_riscv,$(APPS))

all: $(APP_BINS)

%.o: %.s
	$(CC) -I../../../include -c $<

%.eapp_riscv: %.o $(APP_LDS)
	$(LINK) $(LINKFLAGS) -o $@ $< -T $(APP_LDS)
	../../../scripts/parse_elf.sh $@ $*_entry

clean:
	rm -f *.o *.eapp_riscv $(patsubst %,%_entry.h,$(APPS))

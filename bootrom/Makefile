CC = riscv64-unknown-elf-gcc
OBJCOPY= riscv64-unknown-elf-objcopy
CFLAGS = \
	-march=rv64imafdc -mcmodel=medany -mabi=lp64 \
	-nostdlib -nostartfiles -fno-common -std=gnu11 \
	-static \
	-fPIC \
	-O2 -Wall\

# ^ consider taking out -g -Og and putting in -O2

bootloaders=\
	bootrom.elf

.PHONY: all
all: $(bootloaders)

.PHONY: clean
clean:
	rm -f bootrom.elf

bootrom_sources = \
	./bootloader.S \
	./bootloader.c \
	./ed25519/*.c 

%.elf: $(bootrom_sources) bootloader.lds
	$(CC) $(CFLAGS) -I./ -L . -T bootloader.lds -o $@ $(bootrom_sources)

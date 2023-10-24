CROSSCOMPILE = $(shell which riscv${KEYSTONE_BITS}-buildroot-linux-gnu-gcc | rev | cut -c 4- | rev)
CC = $(CROSSCOMPILE)gcc
OBJCOPY = $(CROSSCOMPILE)objcopy

ifeq ($(CROSSCOMPILE),)
$(error RISCV Toochain is not found: riscv${KEYSTONE_BITS}-buildroot-linux-gnu-gcc)
endif

CFLAGS = -Wall -Werror -fno-builtin -nostdlib -g -mcmodel=medany -static $(OPTIONS_FLAGS)
LDFLAGS = -fPIC -nostdlib -g -static $(shell $(CC) --print-file-name=libgcc.a)

SRCS = mem.c string.c printf.c elf.c elf32.c elf64.c loader.c mm.c sbi.c sbi.c vm.c
ASM_SRCS = loader.S
LOADER = loader
LOADER_BIN = loader.bin

OBJS = $(patsubst %.c,obj/%.o,$(SRCS))
ASM_OBJS = $(patsubst %.S,obj/%.S.o,$(ASM_SRCS))
OBJ_DIR_EXISTS = obj/.exists
 
.PHONY: all clean

all: $(LOADER_BIN)

$(LOADER): $(ASM_OBJS) $(OBJS) 
	$(CC) $(LDFLAGS) -T loader.lds $^ -o $@

$(LOADER_BIN): $(LOADER) 
	$(OBJCOPY) -O binary --only-section .text $< loader.bin

$(OBJ_DIR_EXISTS):
	mkdir -p obj
	touch $(OBJ_DIR_EXISTS)

obj/%.S.o: %.S $(OBJ_DIR_EXISTS)
	$(CC) $(CFLAGS) -c $< -o $@

obj/%.o: %.c $(OBJ_DIR_EXISTS)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf obj 
	rm -f $(LOADER) $(LOADER).bin

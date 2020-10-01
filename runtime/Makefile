CROSS_COMPILE = riscv$(BITS)-unknown-linux-gnu-
CC = $(CROSS_COMPILE)gcc
OBJCOPY = $(CROSS_COMPILE)objcopy


ifndef KEYSTONE_SDK_DIR
  $(error KEYSTONE_SDK_DIR is undefined)
endif

CC = riscv64-unknown-linux-gnu-gcc
OBJCOPY = riscv64-unknown-linux-gnu-objcopy
CFLAGS = -Wall -Werror -fPIC -fno-builtin -std=c11 -g $(OPTIONS_FLAGS)
SRCS = aes.c sha256.c boot.c interrupt.c printf.c syscall.c string.c linux_wrap.c io_wrap.c rt_util.c mm.c env.c freemem.c paging.c sbi.c
ASM_SRCS = entry.S
RUNTIME = eyrie-rt
LINK = $(CROSS_COMPILE)ld
LDFLAGS = -static -nostdlib $(shell $(CC) --print-file-name=libgcc.a)

SDK_LIB_DIR = $(KEYSTONE_SDK_DIR)/lib
SDK_INCLUDE_EDGE_DIR = $(KEYSTONE_SDK_DIR)/include/edge
SDK_EDGE_LIB = $(SDK_LIB_DIR)/libkeystone-edge.a

LDFLAGS += -L$(SDK_LIB_DIR)
CFLAGS += -I$(SDK_INCLUDE_EDGE_DIR) -I ./tmplib

DISK_IMAGE = ../busybear-linux/busybear.bin
MOUNT_DIR = ./tmp_busybear

OBJS = $(patsubst %.c,obj/%.o,$(SRCS))
ASM_OBJS = $(patsubst %.S,obj/%.o,$(ASM_SRCS))
OBJ_DIR_EXISTS = obj/.exists

TMPLIB = uaccess.o

.PHONY: all test copy clean

all: $(RUNTIME) $(TEST_BIN)

$(TMPLIB):
	$(MAKE) -C tmplib

$(DISK_IMAGE):
	echo "missing $(DISK_IMAGE)."

copy: $(RUNTIME) $(DISK_IMAGE)
	echo "Copying library $(RUNTIME)"
	mkdir -p $(MOUNT_DIR)
	sudo mount $(DISK_IMAGE) $(MOUNT_DIR)
	sudo cp $(RUNTIME) $(MOUNT_DIR)/lib
	sudo umount $(MOUNT_DIR)
	rm -rf $(MOUNT_DIR)

$(RUNTIME): $(ASM_OBJS) $(OBJS) $(SDK_EDGE_LIB) $(TMPLIB)
	$(LINK) $(LINKFLAGS) -o $@ $^ -T runtime.lds $(LDFLAGS)
	$(OBJCOPY) --add-section .options_log=.options_log --set-section-flags .options_log=noload,readonly $(RUNTIME) 

$(ASM_OBJS): $(ASM_SRCS) $(OBJ_DIR_EXISTS)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR_EXISTS):
	mkdir -p obj
	touch $(OBJ_DIR_EXISTS)

obj/%.o: %.c  $(TMPLIB) $(OBJ_DIR_EXISTS)
	$(CC) $(CFLAGS) -c $< -o $@

test:
	mkdir -p obj/test
	cd obj/test; cmake ../../test
	$(MAKE) -C obj/test
	$(MAKE) -C obj/test test

clean:
	rm -rf $(RUNTIME) obj
	$(MAKE) -C tmplib clean
	# for legacy reasons, remove any lingering uaccess.h
	rm -f uaccess.h

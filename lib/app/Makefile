CC = riscv64-unknown-linux-gnu-gcc
LINK = riscv64-unknown-linux-gnu-ld
LINKFLAGS = -static
INCLUDE_DIR = include
AR = riscv64-unknown-elf-ar
ARFLAGS = rcs

OPTIONS_FLAGS ?=
CFLAGS = -Wall -Werror $(OPTIONS_FLAGS)

ASM_SRCS = src/encret.s
C_SRCS = src/tiny-malloc.c src/string.c src/syscall.c

A_OBJS = $(patsubst %.s,%.o,$(ASM_SRCS))
C_OBJS = $(patsubst %.c,%.o,$(C_SRCS))

TARGET = ../libkeystone-eapp.a

all: $(TARGET)

$(A_OBJS): %.o : %.s
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@
$(C_OBJS): %.o: %.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@
$(TARGET): $(A_OBJS) $(C_OBJS)
	$(AR) $(ARFLAGS) $@ $^


clean:
	rm -f $(A_OBJS) $(C_OBJS) $(TARGET)

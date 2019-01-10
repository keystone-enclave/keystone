CC = riscv64-unknown-linux-gnu-gcc
INCLUDE_DIR = include/
SRC_DIR = src

CCFLAGS = -Wall -Werror -I$(INCLUDE_DIR)

SRCS = src/edge_call.c src/edge_dispatch.c
OBJS = $(patsubst %.c,%.o,$(SRCS))

AR = riscv64-unknown-elf-ar
ARFLAGS = rcs

TARGET=../libkeystone-edge.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJS): %.o: %.c
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 

CC = riscv64-unknown-linux-gnu-gcc
INCLUDE_DIR = include/
SRC_DIR = src

CCFLAGS = -I $(INCLUDE_DIR)

SRCS = src/edge_call.c
OBJS = $(patsubst %.c,%.o,$(SRCS))

AR = riscv64-unknown-elf-ar
ARFLAGS = rcs

TARGET=../libkeystone-edge.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJS): %.o: %.c
	$(CC) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET) 

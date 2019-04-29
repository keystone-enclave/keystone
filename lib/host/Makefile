CC = riscv64-unknown-linux-gnu-g++
INCLUDE_DIR = include/
SRC_DIR = src

OPTIONS_FLAGS ?=

CCFLAGS = -Wall -Werror -I $(INCLUDE_DIR) $(OPTIONS_FLAGS)
SRCS = keystone.cpp elffile.cpp elf.cpp elf32.cpp elf64.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
AR = riscv64-unknown-linux-gnu-ar
ARFLAGS = rcs

TARGET=../libkeystone-host.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

$(OBJS): %.o: $(addprefix $(SRC_DIR)/, %.cpp)
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET)

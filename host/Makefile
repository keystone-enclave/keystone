CC = riscv64-unknown-linux-gnu-g++
INCLUDE_DIR = include/
SRC_DIR = src
CCFLAGS = -I $(INCLUDE_DIR)
SRCS = keystone.cpp
OBJS = $(patsubst %.cpp,%.o,$(SRCS))
AR = riscv64-unknown-elf-ar
ARFLAGS = rcs

TARGET=libkeystone.a

all: $(TARGET)

$(TARGET): $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

%.o: $(addprefix $(SRC_DIR)/, %.cpp)
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f $(OBJS) $(TARGET) 

CC = riscv64-unknown-linux-gnu-g++
APP_DIR = apps/
SRC_DIR = src/
INCLUDE_DIR = include/
CCFLAGS = -I $(INCLUDE_DIR)
SRCS = keystone.cpp
APP_SRCS = hello.c


OBJS = $(patsubst %.cpp,%.o,$(SRCS))

BINS = $(patsubst %.c,%.riscv,$(APP_SRCS))

all: $(OBJS) $(BINS)

%.o: $(addprefix $(SRC_DIR), %.cpp)
	$(CC) $(CCFLAGS) -c $<

%.riscv: $(addprefix $(APP_DIR), %.c) $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

copy: $(BINS)
	$(foreach bin, $^, \
		cd ..; ./copy_sdk_bin.sh $(bin)\
	)

clean:
	rm $(BINS) $(OBJS)


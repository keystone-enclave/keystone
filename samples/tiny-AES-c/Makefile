CC = riscv64-unknown-linux-gnu-g++
OBJCOPY = riscv64-unknown-linux-gnu-objcopy

SDK_LIB_DIR =../../lib
SDK_HOST_LIB = $(SDK_LIB_DIR)/libkeystone-host.a
SDK_INCLUDE_DIR = $(SDK_LIB_DIR)/host/include

RUNNER=aes.riscv
CCFLAGS = -I$(SDK_INCLUDE_DIR)
LDFLAGS = -L$(SDK_LIB_DIR)

SRCS = $(patsubst %.riscv, %.cpp, $(RUNNER))
OBJS = $(patsubst %.riscv, %.o,$(RUNNER)) $(KEYSTONE_OBJ)

all: $(OBJS) $(SDK_HOST_LIB)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(RUNNER) $^

$(SDK_HOST_LIB):
	make -C $(SDK_LIB_DIR)

$(OBJS): %.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o *.riscv

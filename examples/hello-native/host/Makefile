CC = riscv64-unknown-linux-gnu-g++
LD = riscv64-unknown-linux-gnu-ld

SDK_LIB_DIR =$(KEYSTONE_SDK_DIR)/lib

SDK_INCLUDE_HOST_DIR = $(SDK_LIB_DIR)/host/include
SDK_INCLUDE_EDGE_DIR = $(SDK_LIB_DIR)/edge/include

CCFLAGS = -I$(SDK_INCLUDE_HOST_DIR) -I$(SDK_INCLUDE_EDGE_DIR) -I$(SDK_INCLUDE_VERIFIER_DIR) -std=c++11 -Wall -Werror
LDFLAGS = -L$(SDK_LIB_DIR) -static

HOST = host_native
OBJS = $(HOST).o

all: $(HOST)

$(HOST): $(OBJS)
	$(LD) $(LDFLAGS) \
	-o $(HOST) \
	$(OBJS) \
	$(SDK_LIB_DIR)/libkeystone-host.a \
	$(SDK_LIB_DIR)/libkeystone-edge.a

$(OBJS): %.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o $(HOST)

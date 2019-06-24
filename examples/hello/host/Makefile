CC=riscv64-unknown-linux-gnu-g++

all:
	$(CC) -o runner \
		-I$(KEYSTONE_SDK_DIR)/lib/host/include/ \
		-I$(KEYSTONE_SDK_DIR)/lib/edge/include/ \
		host.cpp \
		$(KEYSTONE_SDK_DIR)/lib/libkeystone-host.a \
		$(KEYSTONE_SDK_DIR)/lib/libkeystone-edge.a

CC = riscv64-unknown-linux-gnu-gcc
LD = riscv64-unknown-linux-gnu-ld

SDK_LIB_DIR = $(KEYSTONE_SDK_DIR)/lib

SDK_INCLUDE_APP_DIR = $(SDK_LIB_DIR)/app/include
SDK_INCLUDE_EDGE_DIR = $(SDK_LIB_DIR)/edge/include

CFLAGS += -I$(SDK_INCLUDE_APP_DIR) -I$(SDK_INCLUDE_EDGE_DIR) -Wall -Werror
LDFLAGS = -static -L$(SDK_LIB_DIR)

EAPP = eapp_native
OBJS = $(EAPP).o

all: $(EAPP)

$(EAPP): $(OBJS)
	$(LD) $(LDFLAGS) \
	-o $(EAPP) \
	$(OBJS) \
	$(SDK_LIB_DIR)/libkeystone-eapp.a \
	$(SDK_LIB_DIR)/libkeystone-edge.a
	chmod -x $@

$(OBJS): %.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o $(APP_BIN) $(EXTRA_CLEAN)

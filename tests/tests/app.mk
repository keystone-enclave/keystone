CC := riscv64-unknown-linux-gnu-gcc
CFLAGS := -Wall -Werror
LINK := riscv64-unknown-linux-gnu-ld
AS := riscv64-unknown-linux-gnu-as

O := $(shell pwd)

SDK_LIB_DIR := $(KEYSTONE_SDK_DIR)/lib
SDK_APP_LIB := $(SDK_LIB_DIR)/libkeystone-eapp.a
SDK_INCLUDE_DIR := $(SDK_LIB_DIR)/app/include
SDK_INCLUDE_EDGE_DIR = $(SDK_LIB_DIR)/edge/include

LDFLAGS := -static -L$(SDK_LIB_DIR) -lkeystone-eapp
CFLAGS += -I$(SDK_INCLUDE_DIR)

APP_C_OBJS := $(patsubst %.c,%.o, $(APP_C_SRCS))
APP_A_OBJS := $(patsubst %.s,%.o, $(APP_A_SRCS))
APP_LDS ?= ../app.lds

APP_BIN := $(patsubst %,%.eapp_riscv,$(APP))

APP_C_DST_OBJS := $(addprefix $(O)/,$(APP_C_OBJS))
APP_A_DST_OBJS := $(addprefix $(O)/,$(APP_A_OBJS))
APP_DST_BIN := $(O)/$(APP_BIN)

.PHONY: all clean

all: $(APP_DST_BIN)

$(APP_C_DST_OBJS): $(O)/%.o: %.c
	@mkdir -p $(O)
	$(CC) $(CFLAGS) -c $< -o $@

$(APP_A_DST_OBJS): $(O)/%.o: %.s
	@mkdir -p $(O)
	$(AS) -c $< -o $@

$(APP_DST_BIN): %.eapp_riscv : $(APP_C_DST_OBJS) $(APP_A_DST_OBJS) $(SDK_APP_LIB)
	$(LINK) $(LDFLAGS) -o $@ $^ -T $(APP_LDS)
	chmod -x $@

clean:
	rm -f $(APP_C_DST_OBJS) $(APP_A_DST_OBJS) $(APP_DST_BIN) $(EXTRA_CLEAN)
	rm -d $(O) 2>&1 || true

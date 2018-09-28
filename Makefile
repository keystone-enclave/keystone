CC = riscv64-unknown-linux-gnu-g++
APP_DIR = apps/
SRC_DIR = src/
INCLUDE_DIR = include/
CCFLAGS = -I $(INCLUDE_DIR)
SRCS = keystone.cpp
APP_SRCS = hello.c
ENCLAVE_SRCS= simple_func

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

BINS = $(patsubst %.c,%.riscv,$(APP_SRCS))


DISK_IMAGE = ../busybear-linux/busybear.bin
MOUNT_DIR = ./tmp_busybear


all: $(OBJS) $(BINS)
	$(foreach enclave, $(addprefix $(APP_DIR), $(ENCLAVE_SRCS)),\
		$(MAKE) -C $(enclave) \
	)

%.o: $(addprefix $(SRC_DIR), %.cpp)
	$(CC) $(CCFLAGS) -c $<

%.riscv: $(addprefix $(APP_DIR), %.c) $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

copy: $(BINS)
	mkdir -p $(MOUNT_DIR)
	sudo mount $(DISK_IMAGE) $(MOUNT_DIR)
	$(foreach bin, $^, \
		echo "Copying binary $(bin)"; \
		sudo cp $(bin) $(MOUNT_DIR)/root/ \
	)
	sudo umount $(MOUNT_DIR)
	rmdir $(MOUNT_DIR)

update-header:
	cp ../riscv-linux/arch/riscv/drivers/keystone_user.h ./include/keystone_user.h

clean:
	rm $(BINS) $(OBJS)

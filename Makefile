CC = riscv64-unknown-linux-gnu-g++
HOST_DIR = hosts/
SRC_DIR = src/
BINS_DIR = bins/
INCLUDE_DIR = include/
CCFLAGS = -I $(INCLUDE_DIR)
SRCS = keystone.cpp

HOST_DIRS = $(wildcard $(HOST_DIR)*/)
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

DISK_IMAGE = ../busybear-linux/busybear.bin
MOUNT_DIR = ./tmp_busybear

all: $(OBJS)
	echo $(HOST_DIRS)
	$(foreach enclave, $(HOST_DIRS),\
		$(MAKE) -C $(enclave); \
	)

%.o: $(addprefix $(SRC_DIR), %.cpp)
	$(CC) $(CCFLAGS) -c $<

copy: all
	mkdir -p $(BINS_DIR)
	$(foreach enclave, $(HOST_DIRS),\
		$(MAKE) -C $(enclave) copy;\
	)
	mkdir -p $(MOUNT_DIR)
	sudo mount $(DISK_IMAGE) $(MOUNT_DIR)
	sudo cp $(BINS_DIR)/* $(MOUNT_DIR)/root/;
	sudo umount $(MOUNT_DIR)
	rmdir $(MOUNT_DIR)

update-header:
	cp ../riscv-linux/arch/riscv/drivers/keystone_user.h ./include/keystone_user.h

clean:
	rm $(OBJS)
	rm -rf $(BINS_DIR)
	$(foreach enclave, $(HOST_DIRS),\
		$(MAKE) -C $(enclave) clean;\
	)

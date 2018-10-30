CC = riscv64-unknown-linux-gnu-g++
HOST_DIR = hosts/
SRC_DIR = src/
ENC_SRC_DIR = enc_src/
BINS_DIR = bins/
INCLUDE_DIR = include/
CCFLAGS = -I $(INCLUDE_DIR)
SRCS = keystone.cpp

HOSTS = hello_long hello dummy_loop complex_elf fibonacci fibonacci_bench malloc

HOST_DIRS = $(patsubst %,$(HOST_DIR)%,$(HOSTS))
OBJS = $(patsubst %.cpp,%.o,$(SRCS))

DISK_IMAGE = ../busybear-linux/busybear.bin
MOUNT_DIR = ./tmp_busybear

all: $(OBJS)
	make -C $(ENC_SRC_DIR)
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
	sudo rsync -a $(BINS_DIR)/ $(MOUNT_DIR)/root/
	sudo umount $(MOUNT_DIR)
	rmdir $(MOUNT_DIR)

update-header:
	cp ../riscv-linux/arch/riscv/drivers/keystone_user.h ./include/keystone_user.h

clean:
	rm $(OBJS)
	rm -rf $(BINS_DIR)
	make -C $(ENC_SRC_DIR) clean
	$(foreach enclave, $(HOST_DIRS),\
		$(MAKE) -C $(enclave) clean;\
	)

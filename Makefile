LINUX_DIR=riscv-linux
BUSYBEAR_DIR=busybear-linux
BOOTROM_DIR=bootrom
SM_DIR=riscv-pk
SM_BUILD_DIR=$(SM_DIR)/build
DRIVER_DIR=linux-keystone-driver
SDK_DIR=sdk
QEMU_DIR=riscv-qemu

BOOTROM=$(BOOTROM_DIR)/bootrom.elf
VMLINUX=$(LINUX_DIR)/vmlinux
SM=$(SM_DIR)/build/bbl

DISK=$(BUSYBEAR_DIR)/busybear.bin
DRIVER=$(DRIVER_DIR)/keystone-driver.ko
LINUX_CONFIG=$(LINUX_DIR)/.config
QEMU=$(QEMU_DIR)/riscv64-softmmu/qemu-system-riscv64

NPROC=$(shell nproc)

all: $(BOOTROM) $(VMLINUX) $(SM) $(DISK) $(DRIVER) $(QEMU)
	make -C $(SM_BUILD_DIR)
	make -C $(SDK_DIR)
	make -C $(SDK_DIR) copy-tests
	make -C $(DRIVER_DIR) copy

qemu: all
	./scripts/run-qemu.sh

$(QEMU):
	./scripts/apply-patch.sh
	cd $(QEMU_DIR); ./configure --target-list=riscv64-softmmu,riscv32-softmmu;
	make -C $(QEMU_DIR)

$(SM): $(VMLINUX) $(SM_BUILD_DIR)
	make -C $(SM_BUILD_DIR)

$(SM_BUILD_DIR): $(VMLINUX)
	mkdir -p $(SM_BUILD_DIR)
	cd $(SM_BUILD_DIR); ../configure \
		--enable-logo \
		--host=riscv64-unknown-elf \
		--with-payload=../../$(VMLINUX) \
		--enable-sm

$(DRIVER): $(VMLINUX) $(LINUX_CONFIG)
	make -C $(DRIVER_DIR)

$(DISK):
	make -C $(BUSYBEAR_DIR)
	sudo chmod og+w $(DISK)

$(LINUX_CONFIG):
	cp $(BUSYBEAR_DIR)/conf/linux.config $(LINUX_DIR)/.config
	make -C $(LINUX_DIR) ARCH=riscv olddefconfig

$(VMLINUX): $(LINUX_CONFIG)
	./scripts/apply-patch.sh
	make -C $(LINUX_DIR) ARCH=riscv -j$(NPROC) vmlinux	

$(BOOTROM):
	cd bootrom; make; cd ..

clean:
	rm -f $(LINUX_CONFIG)
	make -C $(LINUX_DIR) ARCH=riscv clean
	make -C $(BUSYBEAR_DIR) clean
	make -C $(BOOTROM_DIR) clean
	make -C $(SM_BUILD_DIR) clean
	make -C $(DRIVER_DIR) clean
	make -C $(QEMU_DIR) clean

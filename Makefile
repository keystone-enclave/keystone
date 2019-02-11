LINUX_SRC_DIR=riscv-linux
BUSYBEAR_DIR=busybear-linux
BOOTROM_DIR=bootrom
SM_DIR=riscv-pk
SM_BUILD_DIR=$(SM_DIR)/build
DRIVER_DIR=$(shell pwd)/linux-keystone-driver
SDK_DIR=sdk
QEMU_DIR=riscv-qemu
LINUX_QEMU_OUT=$(shell pwd)/linux-qemu

BOOTROM=$(BOOTROM_DIR)/bootrom.elf
VMLINUX=$(LINUX_QEMU_OUT)/vmlinux
SM=$(SM_DIR)/build/bbl

DISK=$(BUSYBEAR_DIR)/busybear.bin
DRIVER=$(DRIVER_DIR)/keystone-driver.ko
LINUX_CONFIG=$(LINUX_QEMU_OUT)/.config
QEMU=$(QEMU_DIR)/riscv64-softmmu/qemu-system-riscv64

NPROC=$(shell nproc)

all: $(BOOTROM) $(VMLINUX) $(SM) $(DISK) $(DRIVER) $(QEMU)
	$(MAKE) -C $(SM_BUILD_DIR)
	$(MAKE) -C $(SDK_DIR)
	$(MAKE) -C $(SDK_DIR) copy-tests
	$(MAKE) -C $(DRIVER_DIR) copy

qemu: all
	./scripts/run-qemu.sh

hifive: hifive.mk
	$(MAKE) -f hifive.mk

$(QEMU):
	./scripts/apply-patch.sh
	cd $(QEMU_DIR); ./configure --target-list=riscv64-softmmu,riscv32-softmmu;
	$(MAKE) -C $(QEMU_DIR)

$(SM): $(VMLINUX) $(SM_BUILD_DIR)
	$(MAKE) -C $(SM_BUILD_DIR)

$(SM_BUILD_DIR): $(VMLINUX)
	mkdir -p $(SM_BUILD_DIR)
	cd $(SM_BUILD_DIR); ../configure \
		--enable-logo \
		--host=riscv64-unknown-elf \
		--with-payload=$(VMLINUX) \
		--enable-sm

$(DRIVER): $(VMLINUX) $(LINUX_CONFIG)
	$(MAKE) -C $(LINUX_SRC_DIR) O=$(LINUX_QEMU_OUT) ARCH=riscv SUBDIRS=$(DRIVER_DIR) modules

$(DISK):
	$(MAKE) -C $(BUSYBEAR_DIR)
	sudo chmod og+w $(DISK)

$(LINUX_CONFIG):
	mkdir -p $(LINUX_QEMU_OUT)
	cp $(BUSYBEAR_DIR)/conf/linux.config $(LINUX_CONFIG)
	$(MAKE) -C $(LINUX_SRC_DIR) O=$(LINUX_QEMU_OUT) ARCH=riscv olddefconfig

$(VMLINUX): $(LINUX_CONFIG)
	./scripts/apply-patch.sh
	$(MAKE) -C $(LINUX_SRC_DIR) O=$(LINUX_QEMU_OUT) ARCH=riscv -j$(NPROC) vmlinux

$(BOOTROM):
	cd bootrom; make; cd ..

clean:
	rm -f $(LINUX_CONFIG)
	$(MAKE) -C $(LINUX_SRC_DIR) O=$(LINUX_QEMU_OUT) ARCH=riscv clean
	$(MAKE) -C $(BUSYBEAR_DIR) clean
	$(MAKE) -C $(BOOTROM_DIR) clean
	$(MAKE) -C $(SM_BUILD_DIR) clean
	$(MAKE) -C $(DRIVER_DIR) clean
	$(MAKE) -C $(QEMU_DIR) clean

hifive-clean:
	$(MAKE) -f hifive.mk clean

BUSYBEAR_DIR=busybear-linux
DISK=$(BUSYBEAR_DIR)/busybear.bin
DRIVER_DIR=$(shell pwd)/linux-keystone-driver
DRIVER=$(DRIVER_DIR)/keystone-driver.ko
WORK=$(shell pwd)/busybear-work
LINUX_QEMU_OUT=$(WORK)/linux
LINUX_CONFIG=$(LINUX_QEMU_OUT)/.config
VMLINUX=$(LINUX_QEMU_OUT)/vmlinux
LINUX_SRC_DIR=riscv-linux
SDK_DIR=sdk
SM_DIR=riscv-pk
SM=$(SM_DIR)/build/bbl
SM_BUILD_DIR=$(SM_DIR)/build

all: $(VMLINUX) $(SM) $(DISK) $(DRIVER)
	$(MAKE) -C $(SM_BUILD_DIR)
	$(MAKE) -C $(SDK_DIR) copy-tests
	$(MAKE) -C $(DRIVER_DIR) copy


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


clean:
	rm -f $(LINUX_CONFIG)
	rm -rf $(WORK)
	$(MAKE) -C $(BUSYBEAR_DIR) clean
	$(MAKE) -C $(BOOTROM_DIR) clean
	$(MAKE) -C $(SM_BUILD_DIR) clean
	$(MAKE) -C $(DRIVER_DIR) clean
	$(MAKE) -C $(QEMU_DIR) clean

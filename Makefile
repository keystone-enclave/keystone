BOOTROM_DIR=bootrom
SDK_DIR=sdk
QEMU_DIR=riscv-qemu

BOOTROM=$(BOOTROM_DIR)/bootrom.elf

QEMU=$(QEMU_DIR)/riscv64-softmmu/qemu-system-riscv64

NPROC=$(shell nproc)

all: hifive

qemu:
	./scripts/run-qemu.sh

.PHONY: busybear-qemu
busybear-qemu: $(BOOTROM) $(QEMU) sdk
	$(MAKE) -f busybear.mk

.PHONY: hifive
hifive: sdk $(QEMU) $(BOOTROM)
	$(MAKE) -f hifive.mk
	$(MAKE) -C hifive-work/buildroot_initramfs
	$(MAKE) -f hifive.mk

.PHONY: sdk
sdk:
	$(MAKE) -C $(SDK_DIR)

$(QEMU):
	./scripts/apply-patch.sh
	cd $(QEMU_DIR); ./configure --target-list=riscv64-softmmu,riscv32-softmmu;
	$(MAKE) -C $(QEMU_DIR)

$(BOOTROM):
	cd bootrom; make; cd ..

busybear-clean:
	$(MAKE) -f busybear.mk clean

clean:
	$(MAKE) -f hifive.mk clean

clean-all: busybear-clean clean

BOOTROM_DIR=bootrom
QEMU_DIR=riscv-qemu

BOOTROM=$(BOOTROM_DIR)/bootrom.elf

QEMU=$(QEMU_DIR)/riscv64-softmmu/qemu-system-riscv64

NPROC=$(shell nproc)

all: hifive

qemu:
	./scripts/run-qemu.sh

.PHONY: hifive
hifive: $(QEMU) $(BOOTROM)
	$(MAKE) -f hifive.mk

image:
	$(MAKE) -C hifive-work/buildroot_initramfs
	$(MAKE) -f hifive.mk

run-tests: $(hifive)
	./tests/tests/vault.sh
	$(MAKE) -C hifive-work/buildroot_initramfs
	$(MAKE) -f hifive.mk
	./scripts/travis.sh

$(QEMU):
	./scripts/apply-patch.sh
	cd $(QEMU_DIR); ./configure --target-list=riscv64-softmmu,riscv32-softmmu;
	$(MAKE) -C $(QEMU_DIR)

$(BOOTROM):
	cd bootrom; make; cd ..

sdk:
	$(MAKE) -C sdk

clean:
	$(MAKE) -f hifive.mk clean

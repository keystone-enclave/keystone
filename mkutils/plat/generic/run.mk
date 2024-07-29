######################
## QEMU run targets ##
######################

KEYSTONE_PORT   ?= 9821
QEMU_DBG_PORT   ?= $(shell echo $$(( $(KEYSTONE_PORT) + 1)) )
QEMU_DEBUG      := -gdb tcp::$(QEMU_DBG_PORT) -S

QEMU_MEM        ?= 2G
QEMU_SMP        ?= 4

QEMU_FLAGS := -m $(QEMU_MEM) -smp $(QEMU_SMP) -display none \
                -machine virt,rom=$(BUILDROOT_BUILDDIR)/images/bootrom.bin \
                -bios $(BUILDROOT_BUILDDIR)/images/fw_jump.elf \
                -kernel $(BUILDROOT_BUILDDIR)/images/Image \
                -drive file=$(BUILDROOT_BUILDDIR)/images/rootfs.ext2,format=raw,id=hd0 \
                -device virtio-blk-device,drive=hd0 \
                -append "console=ttyS0 ro root=/dev/vda" \
                -netdev user,id=net0,net=192.168.100.1/24,dhcpstart=192.168.100.128,hostfwd=tcp::$(KEYSTONE_PORT)-:22 \
                -device virtio-net-device,netdev=net0 \
                -device virtio-rng-pci \
                -serial mon:stdio -serial file:/tmp/serial.out

ifneq ($(KEYSTONE_DEBUG),)
        QEMU_FLAGS += $(QEMU_DEBUG)
endif

run:
	$(call log,info,Starting QEMU)
	$(BUILDROOT_BUILDDIR)/host/bin/qemu-system-riscv$(KEYSTONE_BITS) $(QEMU_FLAGS)

debug-connect:
	$(call log,info,Connecting to QEMU)
	$(BUILDROOT_BUILDDIR)/host/bin/riscv$(KEYSTONE_BITS)-buildroot-linux-gnu-gdb \
                -iex "set KEYSTONE=$(KEYSTONE)" \
                -x $(KEYSTONE)/scripts/gdb/generic.cfg

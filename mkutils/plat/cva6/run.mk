######################
## CVA6 Flash SD Card ##
######################



PAYLOAD                 = $(BUILDROOT_BUILDDIR)/images/fw_payload.bin
KERNEL			= $(BUILDROOT_BUILDDIR)/images/uImage
SDDEVICE_PART1          = $(shell lsblk $(SD_DEVICE) -no PATH | head -2 | tail -1)
SDDEVICE_PART2          = $(shell lsblk $(SD_DEVICE) -no PATH | head -3 | tail -1)

flash: $(SD_DEVICE)
	$(info PAYLOAD INFORMATION)
	$(info $(PAYLOAD))
	$(info $(SD_DEVICE))
	$(info $(SDDEVICE_PART1))
	$(info $(SDDEVICE_PART2))
	sgdisk --clear -g --new=1:2048:4M --new=2:512M:0 --typecode=1:3000 --typecode=2:8300 $(SD_DEVICE)
	dd if=$(PAYLOAD) of=$(SDDEVICE_PART1) status=progress oflag=sync bs=1M
	dd if=$(KERNEL) of=$(SDDEVICE_PART2) status=progress oflag=sync bs=1M

debug-connect:
	$(call log,info,Connecting to OpenOCD)
	$(BUILDROOT_BUILDDIR)/host/bin/riscv64-buildroot-linux-gnu-gdb \
                -iex "set KEYSTONE=$(KEYSTONE)" \
                -x $(KEYSTONE)/scripts/gdb/cva6.cfg

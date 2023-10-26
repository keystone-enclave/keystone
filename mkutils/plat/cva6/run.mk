######################
## CVA6 Flash SD Card ##
######################



PAYLOAD                 = $(BUILDROOT_BUILDDIR)/images/fw_payload.bin
SDDEVICE_PART1          = $(SD_DEVICE)1
SDDEVICE_PART2          = $(SD_DEVICE)2



flash: $(SD_DEVICE)
	ifndef SD_DEVICE
		$(error SD_DEVICE is not set)
	endif
	$(info PAYLOAD INFORMATION)
	$(info $(PAYLOAD))
	$(info $(SD_DEVICE))
	$(info $(SDDEVICE_PART1))
	$(info $(SDDEVICE_PART2))
	sgdisk --clear -g --new=1:2048:30M --new=2:512M:0 --typecode=1:3000 --typecode=2:8300 $(SD_DEVICE)
	dd if=$(PAYLOAD) of=$(SDDEVICE_PART1) status=progress oflag=sync bs=1M

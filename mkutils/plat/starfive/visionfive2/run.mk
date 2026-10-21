#########################
## Flash SD card image ##
#########################

DEVICE      ?=
SDIMAGE ?= $(BUILDROOT_BUILDDIR)/images/sdcard.img

flash:
ifeq ($(DEVICE),)
	$(call log,error,Set target device to env DEVICE, e.g. DEVICE=/dev/mmcblk0)
else
	$(call log,info,Flashing SD image)
	sudo dd if=$(SDIMAGE) of=$(DEVICE) bs=4M conv=fdatasync status=progress
endif
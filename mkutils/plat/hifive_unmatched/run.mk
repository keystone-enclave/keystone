#########################
## Flush SD card image ##
#########################

DEVICE      ?=
EXTEND      ?= 0
FLUSH_IMAGE ?= $(BUILDROOT_BUILDDIR)/images/sdcard.img

flush:
ifeq ($(DEVICE),)
	$(call log,error,Set target device to env DEVICE)
else
	$(call log,info,Flushing SD image)
	sudo dd if=$(FLUSH_IMAGE) of=$(DEVICE) bs=64k iflag=fullblock oflag=direct conv=fsync status=progress

ifeq ($(EXTEND),1)
	$(call log,info,Extending rootfs end of the block device)
	echo "w" | sudo fdisk $(DEVICE)
	echo "- +" | sudo sfdisk -N 3 $(DEVICE)
	sudo e2fsck -f $(DEVICE)3
	sudo resize2fs $(DEVICE)3
endif

endif

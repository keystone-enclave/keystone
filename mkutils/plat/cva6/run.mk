######################
## CVA6 Flash SD Card ##
######################

ifndef SD_DEVICE
$(error SD_DEVICE is not set)
endif


PAYLOAD                 := $(BUILDROOT_BUILDDIR)/images/fw_payload.bin
PAYLOAD_SECTOR_START    := 2048
PAYLOAD_SECTOR_START    := $(ls -l --block-size=1 $PAYLOAD | cut -d " " -f5)
PAYLOAD_SECTOR_END      := $(echo $PAYLOAD_SECTOR_START+$PAYLOAD_SECTOR_SIZE | bc)
SDDEVICE_PART1          :=${SDDEVICE}1
SDDEVICE_PART2          :=${SDDEVICE}2

flash:
	sgdisk --clear -g --new=1:$(PAYLOAD_SECTOR_START):30M --new=2:512M:0 --typecode=1:3000 --typecode=2:8300 $(SD_DEVICE)
        dd if=$PAYLOAD of=$SDDEVICE_PART1 status=progress oflag=sync bs=1M



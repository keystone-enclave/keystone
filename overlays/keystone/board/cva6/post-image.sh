#!/bin/sh

if [ $# -ne 2 ]; then
	echo "usage: post-image.sh <imgdir> <xlen>"
	exit 1
fi

if [ $2 -eq 32 ]; then
	UIMAGE_ADDRESS=0x80400000
elif [ $2 -eq 64 ]; then
	UIMAGE_ADDRESS=0x80200000
else
	echo "invalid xlen"
	exit 1
fi

# Generate uboot image
gzip -9 -k --force $1/Image > $1/Image.gz
$BUILDROOT_BUILDDIR/host/bin/mkimage -A riscv -O linux -T kernel -a $UIMAGE_ADDRESS -e $UIMAGE_ADDRESS -C gzip -n "CV$2A6Linux" -d $1/Image.gz $1/uImage

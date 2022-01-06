SiFive HiFive Unleashed Hardware Deployment
===========================================

Building Keystone 
----------------------------------------

Building for the HiFive is straight-forward.
First, clone the Keystone repository in the **manager instance**.

::
  
  git clone https://github.com/keystone-enclave/keystone

Follow :doc:`QEMU-Setup-Repository` to setup the repository.

After you setup the repository, you can run the following commands to build Keystone.

::
  
  mkdir <build directory>
  cd <build directory>
  cmake .. -DLINUX_SIFIVE=y
  make
  make image

CMake with the flag ``-DLINUX_SIFIVE=y`` will automatically generate Makefiles to build
SiFive-compatible Linux and SM.
This includes some patches for DTB compatibility.
Also, the build will forcibly use initramfs for a simpler deployment.

Once you have built the image, you will see ``sm.build/platform/generic/firmware/`` under your
build directory.

Separately, ``make image`` will also generate ``sm.build/platform/generic/firmware/fw_payload.bin`` under your build directory.
This is the file that you want to reflash the SD card with.

You can also boot QEMU machine with the image using ``./scripts/run-qemu.sh``.

Setting up the HiFive
---------------------

Setup Bootloader
################

First, you will need to get a working custom first-stage bootloader
(FSBL) working on your board. This will require creating a new
partition on your SD card as well as setting the MSEL2 dipswitch. See
https://github.com/sifive/freedom-u540-c000-bootloader/issues/9#issuecomment-424162283
for details.

For the bootloader itself, you'll need to build our copy of the bootloader:
https://github.com/keystone-enclave/freedom-u540-c000-bootloader .

Make sure to flash this to the right partition type (see github thread
or example script below).

Load Linux Image
################

The hifive build process generates a bbl.bin in
``<build directory>/sm.build/platform/generic/firmware/fw_payload.bin```. Flash this to the Linux partition on the
card. (Note that this is a relocated version of the bbl binary used
for QEMU)


Example loading script
######################

This is an example of a script to load the FSBL and BBL into a card
for use on the HiFive. Be careful as this will repartition the target
disk!

You only need to reprogram the FSBL when modifying the first-stage
bootloader itself. (Likely never)

::

  #!/bin/bash

  set -e

  # Relevant partition type codes
  BBL=2E54B353-1271-4842-806F-E436D6AF6985
  LINUX=0FC63DAF-8483-4772-8E79-3D69D8477DE4
  FSBL=5B193300-FC78-40CD-8002-E86C45580B47

  MKE2FS=mke2fs
  DISK=$1
  echo "Operating on $DISK"
  test -b $DISK

  echo "$DISK exists, paritioning..."

  # Configure the partitions on the disk.
  # NOTE: The block ranges given here are for the 8GB card we are using
  #       You may wish to use different partition sizes.
  sgdisk --clear \
         --new=1:2048:67583  --change-name=1:bootloader --typecode=1:$BBL   \
         --new=2:264192:     --change-name=2:root       --typecode=2:$LINUX \
         --new=3:67584:69631 --change-name=3:siv-fsbl   --typecode=3:$FSBL \
         $DISK
  sleep 1

  PART_BBL=$DISK"1"
  PART_LINUX=$DISK"2"
  PART_FSBL=$DISK"4"

  echo "COPYING BBL to $PART_BBL"
  test -b $PART_BBL
  dd if=bbl.bin of=$PART_BBL bs=4096
  echo "Copy done"

  echo "COPYING FSBL to $PART_FSBL"
  test -b $PART_FSBL
  $MKE2FS -t ext3 $PART_FSBL
  dd if=fsbl.bin of=$PART_FSBL bs=1024
  echo "Copy done"


Running on the HiFive
---------------------

The needed driver, bins, etc are included in the buildroot image.
You can always use buildroot overlay to add more files to the base image.

Generally, we also suggest ``scp`` ing the files to the board after boot.


Setup network
##########################

Attach to the serial console on the HiFive board.

Boot the HiFive with the custom FSBL/Linux as described above.

Once booted, setup the network such that you can connect to it from
your development machine. (Either a local network or a simple
unmanaged switch is suggested)


Run binaries
#########################

Insert the Keystone driver, and run whatever test binaries you wish.

Example

::

   insmod keystone-driver.ko
   ./tests/tests.ke

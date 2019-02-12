Running Keystone on Hardware
============================

Currently we only support the SiFive HiFive Unleashed development
board (referred to as HiFive for the rest of this document) with an
FU540 chip.

With some reconfiguration of ``riscv-pk`` and ``buildroot`` it should be
possible to build and run Keystone on other Linux-booting RISC-V
platforms.


Building for HiFive
-------------------

Building for the HiFive is straight-forward, run ``make hifive``.

This will build a new copy of the kernel, driver, and generate a full
buildroot Linux image.

All HiFive specific build elements are done in ``keystone/hifive-work``.


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
``hifive-work/bbl.bin``. Flash this to the Linux partition on the
card.


Example loading script
######################

This is an example of a script to load the FSBL and BBL into a card
for use on the HiFive. Be careful as this will repartition the target
disk!

You only need to reprogram the FSBL when modifying

::

  #!/bin/bash

  set -e

  # Relevant partition type codes
  BBL=2E54B353-1271-4842-806F-E436D6AF6985
  LINUX=0FC63DAF-8483-4772-8E79-3D69D8477DE4
  FSBL=5B193300-FC78-40CD-8002-E86C45580B47

  MKE2FS=/usr/local/opt/e2fsprogs/sbin/mke2fs
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

  echo "Copying FSBL"

  # Setup the FSBL FS and copy the bin into it
  $MKE2FS -t ext3 $PART2
  echo "COPYING FSBL to $PART_FSBL"
  dd if=fsbl.bin of=$PART_FSBL

  echo "Copying Linux image"

  # Copy the Linux image in
  echo "COPYING BBL to $PART_BBL"
  dd if=bbl.bin of=$PART_BBL bs=4096


Running on the HiFive
---------------------

Our default build process does not copy any of the binaries or driver
onto the Linux image.

You will need to ``scp`` these files over to the board.

You can also copy these into the HiFive initramfs work directory and
rebuild the image to avoid setting up the network/etc.

Setup network
#############

Attach to the serial console on the HiFive board.

Boot the HiFive with the custom FSBL/Linux as described above.

Once booted, setup the netowrk such that you can connect to it from
your development machine. (Either a local network or a simple
unmanaged switch works)

Copy Files
##########

You'll want to copy over the driver, runtime, and any binaries you
wish to run.

Ex: to run SDK tests on a board available at 10.10.10.3

::

  mkdir hifive-bins
  cp keystone/keystone/sdk/runtime/eyrie-rt ~/keystone/keystone/hifive-work/linux-keystone-driver/*.ko ~/keystone/keystone/sdk/bin/* hifive-bins/

  scp -o "UserKnownHostsFile /dev/null" hifive-bins/* root@10.10.10.3:


Run binaries
############

Insert the Keystone driver, and run whatever test binaries you wish.

Example
::

   insmod keystone-driver.ko
   ./test.sh

CVA6 Hardware Deployment
===========================

Building Keystone
----------------------------------------

Building for the CVA6 is straight-forward.
First, clone the Keystone repository in the **manager instance**.

::

  git clone https://github.com/keystone-enclave/keystone

Follow :doc:`QEMU-Setup-Repository` to setup the repository.

After you setup the repository, you can run the following commands to build Keystone.

::
  export BUILD_ROOT=<absolute path to build directory>
  mkdir <build directory>
  cd <build directory>
  cmake .. -Dcva6=y
  make
  make image

CMake with the flag ``-Dcva6=y`` will automatically generate Makefiles to build
CVA6-compatible Linux and SM. The build will forcibly use initramfs for a simpler deployment.

Once you have built the image, you will see ``sm.build/platform/generic/firmware/fw_payload.bin`` 
under your build directory. This is the file that you want to reflash the SD card with.

You can also boot QEMU machine with the image using ``./scripts/run-qemu.sh``.

Setting up the CVA6
---------------------

FPGA Emulation / Software Simulation
####################################

Keystone can either be simulated (https://github.com/openhwgroup/cva6#memory-preloading) or FPGA emulated (https://github.com/openhwgroup/cva6#corev-apu-fpga-emulation) on the Xilinx Genesys 2 board 
(https://digilent.com/reference/programmable-logic/genesys-2/reference-manual)


Load Linux Image
################

The CVA6 build process generates a fw_payload.bin in
``<build directory>/sm.build/platform/generic/firmware/fw_payload.bin``. Flash this to the Linux partition on the SD
card. (Note that this is a relocated version of the OpenSBI binary used for QEMU)


Example loading script
######################

This is an example of a script to load the FSBL and OpenSBI into a card
for use on the CVA6. Be careful as this will repartition the target disk!

You only need to reprogram the FSBL when modifying the first-stage
bootloader itself. (Likely never)


The CVA6 image uses a simplified structure due to bootloader ROM integration on the device:
:: code-block:: bash
#!/bin/bash

set -e

# format disk
sgdisk --clear --new=1:2048:67583 --new=2 --typecode=1:3000 --typecode=2:8300 /dev/sdc

# flash image
dd if=sm.build/platform/generic/firmware/fw_payload.bin  of=/dev/sdc1 status=progress oflag=sync bs=1M


Running on the CVA6
---------------------

The needed driver, bins, etc are included in the buildroot image.
You can always use buildroot overlay to add more files to the base image.

Generally, we also suggest ``scp`` ing the files to the board after boot.


Run binaries
#########################

Insert the Keystone driver, and run whatever test binaries you wish.

Example

::

   insmod keystone-driver.ko
   ./tests.ke

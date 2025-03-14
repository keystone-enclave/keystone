SiFive HiFive Unmatched Hardware Deployment
===========================================

.. warning::
    Building for the HiFive Unmatched using this approach is experimental.

Preparing freedom-u-sdk
-----------------------

First, obtain the `freedom-u-sdk <https://github.com/sifive/freedom-u-sdk>`_. It is recommended to use the ``2022.12.00`` release since it uses Linux ``5.19.14``, which is close to the default Linux version of the *Keystone* project. Attempts at using Linux ``6.5-rc3`` resulted in a *User space access Oops*.

To create a Kernel Configuration we merge the *freedom-u-sdk* defconfig under ``recipes-kernel/linux/linux-sifive/unmatched/defconfig`` with the *Keystone* sifive defconfig under ``conf/linux64-sifive-defconfig``, giving precedence to *freedom-u-sdk* where necessary. We copy the new defconfig to the aforementioned location inside the *freedom-u-sdk* and in *Keystone* to ``conf/linux64-defconfig``. Note, for *Keystone*, we add ``HAVE_STACKPROTECTOR=n`` (or comment ``select HAVE_STACKPROTECTOR`` out in ``linux/arch/riscv/Kconfig``), as there will be unknown symbols in ``keystone-driver.ko`` otherwise.

Build the image (this will take a while):

::

    kas build freedom-u-sdk/scripts/kas/unmatched.yml --target demo-coreip-cli

and flash the SD Card

::

    xzcat demo-coreip-cli-unmatched.wic.xz | sudo dd of=/dev/mmcblk0 bs=512K iflag=fullblock oflag=direct conv=fsync status=progress


Building Keystone 
-----------------

On the build system, run ``./fast_setup.sh``.

Once completed, replace Linux with version `5.19.14 <https://git.kernel.org/pub/scm/linux/kernel/git/stable/linux.git/tree/?h=linux-5.19.y&id=30c780ac0f9fc09160790cf58f07ef3b92097ceb>`_ such that the kernel and kernel driver match.

Furthermore, update ``OpenSBI`` inside ``sm/`` to the newest available version. 

Use the platform configuration files in ``opensbi/platform/generic`` to update the platform configuration files in ``sm/plat/generic`` (at the time of this writing a PR is pending).

In ``sm/plat/generic/platform.c``, in function ``generic_final_init``, add ``sm_init(cold_boot)`` such that:

::
   
   static int generic_final_init(bool cold_boot)
   {
       void *fdt;
       int rc;
       
       if (generic_plat && generic_plat->final_init) {
       	rc = generic_plat->final_init(cold_boot, generic_plat_match);
       	if (rc)
       		return rc;
       }
       
       sm_init(cold_boot);
       
       if (!cold_boot)
       	return 0;
       
       fdt = fdt_get_address();
       
       fdt_cpu_fixup(fdt);
       fdt_fixups(fdt);
       fdt_domain_fixup(fdt);
       
       if (generic_plat && generic_plat->fdt_fixup) {
       	rc = generic_plat->fdt_fixup(fdt, generic_plat_match);
       	if (rc)
       		return rc;
       }
       
       return 0;
    }


After, you can run the following commands to build *Keystone*.

::
  
  mkdir <build directory>
  cd <build directory>
  cmake .. 
  make
  make image
  make tests

This will automatically build a generic SM.

Once you have built the image, you will see ``sm.build/platform/generic/firmware/`` under your
build directory.

Separately, ``make image`` will also generate ``sm.build/platform/generic/firmware/fw_dynamic.bin`` under your build directory.
This is the file that you want to reflash the SD card with.

Setting up the HiFive
---------------------

Setup Bootloader
################

First, you will need to get a working custom first-stage bootloader
(FSBL) working on your board. This will require creating a new
partition on your SD card as well as setting the MSEL2 dipswitch. See the `Getting Started Guide <https://sifive.cdn.prismic.io/sifive/65937c64-c6ca-4bc7-80d1-e830a895636a_hifive-unmatched-getting-started-guide-v1p4.pdf#%5B%7B%22num%22%3A60%2C%22gen%22%3A0%7D%2C%7B%22name%22%3A%22XYZ%22%7D%2C0%2C125%2C0%5D>`_
for details.

For the bootloader itself, you'll need to use ``u-boot``: 

https://github.com/u-boot/u-boot.

Build according to the `u-boot build instructions <https://u-boot.readthedocs.io/en/latest/board/sifive/unmatched.html>`_ using the *Keystone* custom *OpenSBI*:

::

    export OPENSBI=<path to keystone/build/sm.build/platform/generic/firmware/fw_dynamic.bin>
    cd <U-Boot-dir>
    make sifive_unmatched_defconfig
    make FW_PAYLOAD=<path to keystone/build/sm.build/platform/generic/firmware/fw_payload.bin>

Program the SPL and *u-boot* on the card where the pre-built image is already flashed.

::

    sudo dd if=spl/u-boot-spl.bin of=/dev/mmcblk0 seek=34
    sudo dd if=u-boot.itb of=/dev/mmcblk0 seek=2082

Running on the HiFive
---------------------

Attach to the serial console on the HiFive board.

Boot the HiFive with the custom FSBL/Linux as described above.

Copy Keystone driver and examples
#################################

Copy the *Keystone* driver from ``sm.build/linux-keystone-driver.build/keystone-driver.ko`` and the examples from ``sm.build/examples`` to the board.
Since we used the *freedom-u-sdk*, you have to copy the files manually.

Run binaries
#########################

Insert the *Keystone* driver, and run whatever test binaries you wish.

Example

::

    insmod keystone-driver.ko
    ./tests/tests.ke 




The ``fib-bench`` test causes the system to hang since it uses the `rdcycle` instruction which is now illegal from `u-mode`, hence, should be replaced by `rdtime`,  and ``attestation`` fails as the required values are absent during the SM initialization.


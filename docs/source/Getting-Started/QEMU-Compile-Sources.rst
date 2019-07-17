Compile Sources
-----------------------------

Build All
#################

If you want to build all, simply run ``make``. This also rebuilds any
modifications.

``PATH`` must include the RISC-V tool path.

::

  make

If you want to manually build each individual component, please follow
the instructions below. If you run into any issues, check our
``Makefile`` and ``hifive.mk`` as they will always have up-to-date
build instructions.

Otherwise, skip to :ref:`LaunchQEMU`.

Build Buildroot
########################

This is handled as part of the top-level make, see ``hifive.mk`` for
details.

Buildroot configuration is in ``hifive-conf/buildroot_initramfs_config``.

Build RISC-V QEMU
##################

You should apply patches before building QEMU/Linux.

::

  ./scripts/apply-patch.sh


::

  cd riscv-qemu
  ./configure --target-list=riscv64-linux-user,riscv64-softmmu,riscv32-linux-user,riscv32-softmmu
  make
  cd ..

Build Linux Kernel
################################################

This is handled as part of the top-level make, see ``hifive.mk`` for
details.

Kernel configuration is in ``hifive-conf/linux_cma_config``.


Build Berkeley Bootloader (BBL) with Keystone Security Monitor
##############################################################

This is handled as part of the top-level make, see ``hifive.mk`` for
details.

Optionally, add ``--with-target-platform=PLATFORM`` if you have a
platform specific set of files for the security monitor (defined in ``riscv-pk/sm/platform/``). See
:doc:`../Building-Components/Security-Monitor-Platform-Build` for details.

Build Root-of-Trust Boot ROM
###############################

::

  cd bootrom
  make
  cd ..

Build Keystone Driver
##############################

This is handled as part of the top-level make, see ``hifive.mk`` for
details.
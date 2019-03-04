Running Keystone with QEMU
====================================

`QEMU <https://www.qemu.org>`_ is an open source machine emulator.
The latest QEMU supports RISC-V ISA.

Keystone is tested in the latest RISC-V QEMU (`GitHub <https://github.com/riscv/riscv-qemu>`_).
The upstream QEMU might not work because it has a bug in the PMP module (`See GitHub issue <>`.
The fix will be upstreamed in the future.

Installing Dependencies
----------------------------

We tested Keystone with QEMU Ubuntu 16.04/18.04 and derivatives.

Ubuntu
#######################

::

  sudo apt update
  sudo apt install autoconf automake autotools-dev bc bison build-essential curl \
  expat libexpat1-dev flex gawk gcc git gperf libgmp-dev libmpc-dev libmpfr-dev \
  libtool texinfo tmux patchutils zlib1g-dev wget bzip2 patch vim-common lbzip2 \
  python pkg-config libglib2.0-dev libpixman-1-dev libssl-dev device-tree-compiler

.. note::

    Some of the utilities also use ``expect`` so we recommend that you install that as well though it is not strictly necessary.
    ::
      sudo apt install expect
  
Quick Setup
----------------------------

In this stage, you will (1) install RISC-V toolchain, and (2) checkout git submodules.

You can quickly setup everything by running ``./fast-setup.sh``
::

  ./fast-setup.sh

NOTE: the prebuilt toolchain in fast-setup is known to have problems
on Ubuntu 18.04 due to library versioning mismatches.

This will download pre-compiled RISC-V tools and extract it to ``riscv`` directory.

If you want to compile RISC-V tools from source code, run ``./setup.sh`` instead.

To keep environment variables, add export PATH=$PATH:<path/to/keystone>/riscv/bin to your .bashrc. You can also manually run ``source source.sh`` to set the environment variables.


Compile Sources
-----------------------------

Build All
########################

If you want to build all, simply run ``make hifive``.

(You may run ``make`` which will build a Busybear based qemu-only
image, this is in the process of being depricated and eventually
``make hifive`` will become the default)

``PATH`` must include the RISC-V tool path.

::

  make hifive

If you want to manually build each individual component, please follow the instructions below.
Otherwise, skip to :ref:`LaunchQEMU`.

.. attention::

  Currently, ``make`` requires sudo previlege to build Busybear image.

Build Busybear
################################

See `Busybear repo <https://github.com/michaeljclark/busybear-linux>`_ for more information.

We are in the process of deprecating all busybear based builds.

::

  cd busybear-linux
  make
  cd ..

Build RISC-V QEMU
##################

You should apply patches before building the QEMU.

::

  ./scripts/apply-patch.sh
  cd riscv-qemu
  ./configure --target-list=riscv64-softmmu,riscv32-softmmu
  make
  cd ..

Build Linux Kernel
################################################

::

  cd riscv-linux
  cp ../busybear-linux/conf/linux.config .config
  make ARCH=riscv olddefconfig
  make ARCH=riscv vmlinux
  cd ..

Build Berkeley Bootloader (BBL) with Keystone Security Monitor
##############################################################

Make sure to add ``--enable-sm`` when you run ``configure`` so that the security monitor is included in the bbl.

::

  cd riscv-pk
  mkdir build
  cd build
  ../configure \
      --enable-logo \
      --host=riscv64-unknown-elf \
      --with-payload=../../riscv-linux/vmlinux \
      --enable-sm
  make
  cd ../..

Build Root-of-Trust Boot ROM
###############################

::

  cd bootrom
  make
  cd ..

Build Keystone Driver
##############################

::

  cd linux-keystone-driver
  make
  make copy
  cd ..

Build Keystone SDK
#############################

Keystone SDK includes sample enclave programs and some useful libraries. To run sample programs, you should compile SDK library and apps, and copy all of them into the disk image. Following commands will compile the sdk, and copy sample binaries into the ``busybear.bin`` disk image.

::

  cd sdk
  make
  make copy-tests
  cd ..


.. _LaunchQEMU:

Launch QEMU
--------------------------------------

Now, you're ready to run Keystone.

The following script will run QEMU, start executing from the emulated silicon root of trust.
The root of trust then jumps to the SM, and the SM boots Linux!

::

   ./scripts/run-hifive-qemu.sh

Login as ``root`` with the password ``sifive``.


Or if you want to run the busy-bear based image

::

   sudo chmod og+w busybear-linux/busybear.bin
  ./scripts/run-qemu.sh

Login as ``root`` with the password ``busybear``.

You can exit QEMU by ``ctrl-a``+``x`` or using ``poweroff`` command

Insert Keystone Driver
##################################

Insert the keystone driver.

::

    insmod keystone-driver.ko

Run Tests
##################################

You can run Keystone enclaves by using an untrusted host application. We already implemented a simple host ``test-runner.riscv`` for running tests.
Following command will create and execute the enclave.

You can find each of the test enclave in ``sdk/tests/<name>``

Currently, Keystone is only compatible with a prototype runtime, ``eyrie-rt``, which you can find in ``sdk/runtime``.

::

  ./test-runner.riscv <user elf> <runtime elf>

To run all tests, you could simply run

::

  ./test

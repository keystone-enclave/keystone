Running Keystone with QEMU
====================================

`QEMU <https://www.qemu.org>`_ is an open source machine emulator.
The latest QEMU supports RISC-V ISA.

Keystone is tested in the latest RISC-V QEMU (`GitHub <https://github.com/riscv/riscv-qemu>`_).
The upstream QEMU might not work because it has a bug in the PMP module (`See GitHub issue <>`.
The fix will be upstreamed in the future.

Installing Dependencies
----------------------------

We tested Keystone with QEMU on CentOS and Ubuntu 16.04/18.04

Cent OS
###########

::

  sudo yum install autoconf automake autotools-dev bc bison build-essential \
  curl expat expat-devel flex gawk gcc gcc-c++ git gperf libgmp-dev libmpc-dev \
  libmpfr-dev libtool mpfr-devel texinfo tmux patchutils zlib1g-dev zlib-devel \
  wget bzip2-devel lbzip2 patch

Ubuntu
#######################

::

  sudo apt update
  sudo apt install autoconf automake autotools-dev bc bison build-essential curl \
  expat libexpat1-dev flex gawk gcc git gperf libgmp-dev libmpc-dev libmpfr-dev \
  libtool texinfo tmux patchutils zlib1g-dev wget bzip2 patch vim-common lbzip2 \ 
  python pkg-config libglib2.0-dev libpixman-1-dev


Compile Sources
-----------------------------

Clone the repository
########################

::
  
  git clone https://github.com/keystone-enclave/keystone
  cd keystone
  git submodule update --init --recursive


Install RISC-V GNU Toolchain
############################

::
  
  mkdir riscv
  export RISCV=$(pwd)/riscv
  export PATH=$PATH:$RISCV/bin
  cd riscv-gnu-toolchain
  ./configure --prefix=$RISCV
  make && make linux
  cd ..

This step installs RISC-V GNU toolchain in the ``keystone/riscv`` directory.

To keep environment variables, add ``export PATH=$PATH:<path/to/keystone>/riscv/bin`` to your ``.bashrc``.
You can also manually run ``source source.sh`` to set the environment variables.

Create Disk Image using Busybear 
################################

See `Busybear repo <https://github.com/michaeljclark/busybear-linux>`_ for more information.

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

Build Linux Kernel with Built-in Keystone Driver
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

Build Keystone SDK
#############################
Keystone SDK includes sample enclave programs and some useful libraries. To run sample programs, you should compile SDK library and apps, and copy all of them into the disk image. Following commands will compile the sdk, and copy sample binaries into the ``busybear.bin`` disk image.

::

  cd sdk
  make
  make copy-tests
  cd ..


Launch QEMU
--------------------------------------

Now, you're ready to run Keystone.

The following script will run QEMU, start executing from the emulated silicon root of trust.
The root of trust then jumps to the SM, and the SM boots Linux!

::

  ./scripts/run-qemu.sh

Login as ``root`` with the password ``busybear``.

You can exit QEMU by ``ctrl-a``+``x``

Run Tests
---------------------------------------

You can run Keystone enclaves by using an untrusted host application. We already implemented a simple host ``test-runner.riscv`` for running tests.
Following command will create and execute the enclave.

::

  ./test-runner.riscv <user elf> <runtime elf>

To run all tests, you could simply run

::

  ./test



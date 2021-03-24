Compile Sources
-----------------------------

Set up Environment
##############################################################

After you clone the repo and have checked out the `dev-embedded-freeRTOS` branch,
run

::

  BITS=32 ./fast-setup.sh

This will download and install the rv32 toolchain.

After this, you should check to see if the ``riscv32-unknown-linux-gnu-gcc`` is in your path.

Build All Components
##############################################################

We use `CMake <https://cmake.org/>`_ as a build system. It lets you generate the Makefile for a
given configuration.

``PATH`` must include the RISC-V toolchain.

::

  mkdir <build directory>
  cd <build directory>
  cmake .. -DRISCV32=y -DOPERATING_SYSTEM=freeRTOS
  make

Here are some useful CMake flags you can add:

* ``-Dinitramfs=y``: This will compile the Linux kernel with buildroot image as the initramfs. NOT SUPPORTED
* ``-DRISCV32=y``: This will conrigure the build with RV32. You should have ran ``BITS=32 ./fast-setup.sh``.
* ``-DLINUX_SIFIVE=y``: This is a temporary flag for HiFive Unleashed board.
* ``-DSM_PLATFORM=<platform>``: The security monitor will be compiled with platform sources in ``sm/plat/<platform>``. The default value is "generic". NOT SUPPORTED
* ``-DUSE_RUST_SM=y``: Use the Rust port of the security monitor. Curently not supported in v1.0. NOT SUPPORTED
* ``-DOPERATING_SYSTEM=<OS>``: This specifies the operating system to build. Currently only ``linux`` and ``freeRTOS`` are supported
* ``-DCMAKE_BUILD_TYPE=Debug``: This will build everying in debug mode which assists in GDB debugging. 

In order to build the source code, and have the final images for QEMU, you need to run

::

  # in your <build directory>
  make

If you run into any issues, check our
``CMakeLists.txt`` and as it will always have the up-to-date build recipes.

Please refer to the following sections if you want to learn how to build each individual component.
Otherwise, skip to :ref:`LaunchQEMU`.

Running the built files with QEMU
##############################################################

Once you have done that, you can run QEMU on the emulator with:

::

  ./scripts/run-qemu.sh

Updating Images
##############################################################

Once you have built every component, you may need to update some of the build files.

To do this, you only need to rerun make.

::

  # in your <build directory>
  make

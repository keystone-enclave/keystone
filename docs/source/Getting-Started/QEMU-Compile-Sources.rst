Compile Sources
-----------------------------

Build All Components
##############################################################

We use `CMake <https://cmake.org/>`_ as a build system. It lets you generate the Makefile for a
given configuration.

``PATH`` must include the RISC-V toolchain.

::

  mkdir <build directory>
  cd <build directory>
  cmake ..
  make

Here are some useful CMake flags you can add:

* ``-Dinitramfs=y``: This will compile the Linux kernel with buildroot image as the initramfs.
* ``-DRISCV32=y``: This will conrigure the build with RV32. You should have ran ``BITS=32 ./fast-setup.sh``.
* ``-DLINUX_SIFIVE=y``: This is a temporary flag for HiFive Unleashed board.
* ``-DSM_PLATFORM=<platform>``: The security monitor will be compiled with platform sources in ``sm/plat/<platform>``. The default value is "generic".
* ``-DUSE_RUST_SM=y``: Use the Rust port of the security monitor. Curently not supported in v1.0.

In order to build the driver and have the final images for QEMU, you need to run

::

  # in your <build directory>
  make image

If you run into any issues, check our
``CMakeLists.txt`` and as it will always have the up-to-date build recipes.

Please refer to the following sections if you want to learn how to build each individual component.
Otherwise, skip to :ref:`LaunchQEMU`.

Build Buildroot
##############################################################

Buildroot config files are located at ``conf/``. RV64 buildroot will use ``conf/qemu_riscv64_virt_defconfig``.
The following command will build buildroot with the config:

::

  # in your <build directory>
  make buildroot

Build QEMU (in-tree)
##############################################################

We do not try to build QEMU out-of-tree as it will unlikely to be rebuilt once it's compiled.
The in-tree compilation will take place in ``qemu``.
Thus, rebuilding QEMU may require ``make clean`` in ``qemu``.

Keystone requires patches for QEMU (1) to emulate the secure boot via on-chip bootrom and (2) to
apply not-yet-upstreamed bug fixes. All patches are located at ``patches/qemu/``

The following command will configure and build QEMU after applying the patches:

::

  # in your <build directory>
  make qemu

Build Linux Kernel
##############################################################

Kernel config files are located at ``conf/``. RV64 linux will use ``conf/linux-v5.0-defconfig-rv64``.
The following command will build the linux.
Note that you need at least 2GB of memory in order to successfully build the kernel.

Keystone requires patches for the Linux kernel to reserve CMA region at boot.
The patch is located at ``patches/linux/``.

::

  # in your <build directory>
  make linux

Build OpenSBI Firmware with Keystone Security Monitor
##############################################################

The following command will build the M-mode security monitor.

::

  # in your <build directory>
  make sm

Build Root-of-Trust Boot ROM
##############################################################

This is used for secure boot. With our patch for QEMU, compiled boot code will be copied to the boot
ROM in QEMU.

::

  # in your <build directory>
  make bootrom

Build Keystone Driver
##############################################################

Linux module does not support in-tree build.
Thus, we ``rsync`` the entire source code directory with a build directory and build the driver there.

The following command will build the linux driver for Keystone.

::

  # in your <build directory>
  make driver

Updating Images
##############################################################

Once you have built every component, you may need to update the final buildroot image by running

::

  # in your <build directory>
  make image

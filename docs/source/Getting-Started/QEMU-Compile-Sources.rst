Compile Sources
-----------------------------

Build All Components (RV64)
##############################################################

We use `CMake <https://cmake.org/>`_ as a build system. It lets you generate the Makefile for a
given configuration.

``PATH`` must include the RISC-V tool path.

::

  mkdir <build directory>
  cd <build directory>
  cmake ..
  make

In order to build the driver and the tests, and have the final images for QEMU, you need to run

::

  # in your <build directory>
  make image

This updates all components except QEMU and the boot ROM.

If you want to use ``initramfs``, use the following flag when running ``cmake``

::

  cmake .. -Dinitramfs=y

Please refer to the following sections if you want to learn how to build each individual component.
Otherwise, skip to :ref:`LaunchQEMU`.

Most of the components will be built out-of-tree; in ``<build directory>/<component name>.build``
directory.

If you run into any issues, check our
``CMakeLists.txt`` and as it will always have the up-to-date build recipes.


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
The following command will build the linux 

Keystone requires patches for the Linux kernel to reserve CMA region at boot.
The patch is located at ``patches/linux/``

::

  # in your <build directory>
  make linux

Build Berkeley Bootloader (BBL) with Keystone Security Monitor
##############################################################

The following command will build the M-mode security monitor.

::

  # in your <build directory>
  make sm

Optionally, you can specify the target platform when you run ``cmake``.

::

  # in your <build directory>
  cmake .. -Dplatform=fu540
  make sm

The default platform is "default", which does not have any platform-specific features.
See ``riscv-pk/sm/platform/`` for available platforms. 
See :doc:`../Building-Components/Security-Monitor-Platform-Build` for details.

Build Root-of-Trust Boot ROM
##############################################################

This is used for secure boot. With our patch for QEMU, compiled boot code will be copied to the boot
ROM in QEMU. 

::

  # in your <build directory>
  make bootrom

Build Keystone Driver (in-tree)
##############################################################

Linux module does not support in-tree build. Rebuilding the driver may require ``make clean`` in
``linux-keystone-driver``.

The following command will build the linux driver for Keystone.

::

  # in your <build directory>
  make driver

Build Tests (in-tree)
##############################################################

The tests will be built in-tree. We will make it out-of-tree soon.
Rebuilding the tests may require ``make clean`` in ``tests/tests``.

The following command will build the tests

::

  # in your <build directory>
  make tests


Updating Images
##############################################################

Once you have built every component, you may need to update the final images by running

::

  # in your <build directory>
  make image

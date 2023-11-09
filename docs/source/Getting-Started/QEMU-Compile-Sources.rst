Compile Sources
-----------------------------

Build All Components
##############################################################

.. note::

  Keystone recently transitioned from a CMake build system to what you will find here.
  To view a previous version of the documentation, please see the `GitHub repository <https://github.com/keystone-enclave/keystone/commit/3d0d7a621813b74d0b3da37ce6fc92b08bc04f5e>`_.

We use `Make <https://www.gnu.org/software/make/manual/make.html/>`_ and `Buildroot <https://buildroot.org/>`_ as a build system.
The top-level Makefile is located in the root directory of the repository and is the main frontend to the build system.
It collects configuration options and initiates the build process, which itself takes place in Buildroot.

A build can be configured with the following options (along with their default values). The values must be passed to the build as environment variables, either through an ``export`` in the shell, or by passing them
directly to ``make`` by prepending them to the command (e.g. ``OPTION1=VALUE1 OPTION2=VALUE2 (...) make``):

* ``KEYSTONE_PLATFORM=generic``: Configures the platform to build for. Currently, only the ``generic`` `QEMU virtual platform <https://www.qemu.org/docs/master/system/openrisc/virt.html/>`_ is supported.
* ``KEYSTONE_BITS=64``: Configures if the build is for RV64 or RV32.
* ``BUILDROOT_CONFIGFILE=qemu_riscv$(KEYSTONE_BITS)_virt_defconfig``: Configures the buildroot config file to use.
* ``BUILDROOT_TARGET=all``: Configures the target to be built. (e.g. ``keystone-sm``, for the security monitor)

Use the following command to initiate a build of all components:

::

  make -j$(nproc)

This will build all Keystone components and generate a bootable image for QEMU, placing the
build output in ``build-$PLATFORM$BITS``.

Rebuilding Changed Components
##############################################################

In a very common workflow, a developer would make changes to a component and then rebuild the component.
As built packages are synced to various other places by Buildroot, this can be prone to stale and thus incorrect builds.

To avoid this, changes to components are detected by using content-adressed versioning of the component sources, allowing
Buildroot to detect builds that include stale sources. In such cases, a warning is printed when executing the build.

A stale source is then removed by using the following command:

::

  BUILDROOT_TARGET=<target>-dirclean make -j$(nproc)

This will remove the stale source directory. Afterwards, a new build can be initiated as usual.

Appendix A: Configuring Buildroot and the Linux Kernel
##############################################################

There are convenience targets in the top-level Makefile to configure both Buildroot and the Linux kernel.

To configure Buildroot, use the following command:

::

  make buildroot-configure

To configure the Linux kernel, use the following command:

::

  make linux-configure

These commands open a menu-based configuration interface. After making changes, save the configuration and exit the interface.
The Makefile takes care of placing the configuration files in the correct locations.

Appendix B: Debugging Failed Builds
##############################################################

As the Buildroot output is very verbose, only certain parts of it are printed to ``stdout`` by default.
The full output of a build is written to ``build-$PLATFORM$BITS/build.log``, which can be used to debug failed builds.

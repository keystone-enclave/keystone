Getting Started
==================================

What is Keystone?
--------------------------------------------

Keystone is an open-source TEE framework for RISC-V processors.

You can currently try Keystone on `QEMU <https://www.qemu.org/>`_, `FireSim <https://fires.im/>`_ (FPGA), or the SiFive `HiFive Unleashed <https://www.sifive.com/boards/hifive-unleashed>`_ board.

You can migrate the Keystone enclave into arbitrary RISC-V processor, with a very small modification on hardware to plant the silicon root of trust.

.. attention::

  We're actively adding more documents right now. Please post in `Keystone forum <https://groups.google.com/forum/#!forum/keystone-enclave-forum>`_ or create a `GitHub issue <https://github.com/keystone-enclave/keystone/issues>`_ if you face any undocumented trouble.

.. attention::

  The current version (0.X) of Keystone is not formally verified, nor matured.
  We recommend you to use Keystone only for research purposes until it gets stablized.
  We appreciate any :doc:`contribution<../Contributing-to-Keystone/How-to-Contribute>` for making Keystone better.


Quick Start
--------------------------------------------

Following documents are quick guide to test or deploy Keystone on various platforms.

.. toctree::
   :maxdepth: 1

   Running-Keystone-with-QEMU
   Running-Keystone-with-FireSim
   Running-Keystone-on-Hardware


Tutorials
--------------------------------------------


* :doc:`Hello world with libc<Tutorials/Build-Enclave-App-Hello-World>`
* :doc:`Hello world without libc<Tutorials/Build-Enclave-App-Hello-World-Native>`
* :doc:`Remote Attestation<Tutorials/Remote-Attestation>`


Keystone Demo
-------------

The Keystone Demo is an example of the current capabilities of the
Keystone enclave framework.

See https://github.com/keystone-enclave/keystone-demo and
documentation contained within.

A Guide to Keystone Components
--------------------------------------------

The Keystone repository (`<https://github.com/keystone-enclave/keystone>`_) consists of a number of sub-components as
gitmodules or directories. This is a brief overview of them.

::

    + keystone/
    |-- patches/
    |  # required patches for submodules
    |-- bootrom/
    |  # Keystone bootROM for QEMU virt board, including trusted boot chain.
    |-- buildroot/
    |  # Linux buildroot. Builds a minimal working Linux image for our test platforms.
    |-- docs/
    |  # Contains read-the-docs formatted and hosted documentation, such as this article.
    |-- riscv-gnu-toolchain/
    |  # Unmodified toolchain for building riscv targets. Required to build all other components.
    |-- linux-keystone-driver/
    |  # A loadable kernel module for Keystone enclave.
    |-- linux/
    |  # Linux kernel
    |-- sm/
    |  # OpenSBI firmware + Keystone security monitor
    |-- qemu/
    |  # QEMU
    +-- sdk/
       # Tools, libraries, and example apps for building enclaves on Keystone

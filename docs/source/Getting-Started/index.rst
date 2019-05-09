Getting Started
==================================

What is Keystone?
-----------------------------------

Keystone is an open-source TEE framework for RISC-V processors.

You can currently run Keystone on qemu, `FireSim <https://fires.im/>`_ (FPGA), or the SiFive `HiFive Unleashed <https://www.sifive.com/boards/hifive-unleashed>`_ board.

You can migrate the Keystone enclave into arbitrary RISC-V processor, with a very small modification on hardware to plant the silicon root of trust.

.. attention::

  We're actively adding more documents right now. Please post in `Keystone forum <https://groups.google.com/forum/#!forum/keystone-enclave-forum>`_ or create a `GitHub issue <https://github.com/keystone-enclave/keystone/issues>`_ if you face any undocumented trouble.

.. attention::

  The current version (0.X) of Keystone is not formally verified, nor matured.
  We recommend you to use Keystone only for research purposes until it gets stablized.
  We appreciate any contribution for making Keystone better.

Quick Start
----------------------------------------
.. toctree::
   :maxdepth: 2

   Running-Keystone-with-QEMU
   Running-Keystone-with-FireSim
   Running-Keystone-on-Hardware

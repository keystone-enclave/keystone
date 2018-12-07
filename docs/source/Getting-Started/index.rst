Getting Started
==================================

What is Keystone Enclave
-----------------------------------

Keystone Enclave is an open source secure enclave for RISC-V processors. 
You can migrate the Keystone enclave into arbitrary RISC-V processor, with a very small modification on hardware to plant the silicon root of trust.

.. attention::

  We're actively adding more documents right now. Please post in `Keystone forum <https://groups.google.com/forum/#!forum/keystone-enclave>`_ or create a `GitHub issue <https://github.com/keystone-enclave/keystone/issues>`_ if you face any undocumented trouble.

.. attention::

  The current version (0.X) of Keystone is not formally verified, nor matured, which means that it might have bugs or unknown security holes.
  We recommend you to use Keystone only for research purposes until it gets stablized.
  We appreciate any contribution for making Keystone better.
  
Since no hardware has been built for Keystone, we provide a few ways to deploy and test Keystone for building secure systems and developing enclave applications.


Quick Start
----------------------------------------
.. toctree::
   :maxdepth: 2

   Running-Keystone-with-QEMU
   Running-Keystone-with-FireSim

How it Works?
-----------------------------------

.. toctree::
   :maxdepth: 2

   RISC-V-Background
   Architecture-Overview
   Memory-Isolation
   Platform-Integration
   What-Is-Runtime
   Remote-Attestation

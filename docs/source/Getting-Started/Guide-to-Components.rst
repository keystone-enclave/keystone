A Guide to Keystone Components
==============================

The Keystone repository (`<https://github.com/keystone-enclave/keystone>`_) consists of a number of sub-components as
gitmodules or directories. This is a brief overview of them.

bootrom
-------

Keystone bootrom, including trusted boot chain.

buildroot
---------

Link: `<https://github.com/keystone-enclave/buildroot>`_

Linux buildroot. Builds a minimal working Linux image for our test platforms.

busybear-linux
--------------

Link: `<https://github.com/keystone-enclave/busybear-linux>`_

Unmodified busybear Linux, supporting riscv. Deprecated. Soon to be
removed.

docs
----

Contains read-the-docs formatted and hosted documentation, such as
this article.

riscv-gnu-toolchain
-------------------

Unmodified toolchain for building riscv targets. Required to build all
other components.

linux-keystone-driver
---------------------

Link: `<https://github.com/keystone-enclave/linux-keystone-driver>`_

A loadable module for Keystone Enclave.
Works in kernels >= 4.17


riscv-linux
-----------

Link: `<https://github.com/riscv/riscv-linux>`_

Linux kernel with riscv patches.
We use 4.15 with patches for the loadable module support.

riscv-pk
--------

Link: `<https://github.com/keystone-enclave/riscv-pk>`_

The proxy-kernel for machine-mode riscv. Trusted core component of
Keystone, and includes the security monitor (in sm directory).

riscv-qemu
----------

Qemu with riscv patches and minor modifications to better support PMP
and Keystone needs for emulation. Our default testing platform.

sdk
---

Link: `<https://github.com/keystone-enclave/keystone-sdk>`_

Tools, libraries, and tests for building applications on Keystone.

Runtime
~~~~~~~

Link: `<https://github.com/keystone-enclave/keystone-runtime>`_

A submodule that implements the default minimal Keystone runtime
running in S-mode for enclaves.


keystone-demo
-------------

Link: `<https://github.com/keystone-enclave/keystone-demo>`_

A complete demo application using Keystone. Hosts an enclave that
performs computation over data provided by a remote client using
secure channels.

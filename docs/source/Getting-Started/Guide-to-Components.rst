A Guide to Keystone Components
==============================

The Keystone repository consists of a number of sub-components as
gitmodules. This is a brief overview of them.

bootrom
-------

Keystone bootrom, including trusted boot chain.

busybear-linux
--------------

Unmodified busybear Linux, supporting riscv. Our default untrusted
Linux OS for testing.

docs
----

Contains read-the-docs formatted and hosted documentation, such as
this article.

riscv-gnu-toolchain
-------------------

Unmodified toolchain for building riscv targets. Required to build all
other components.

riscv-linux
-----------

Linux kernel with riscv patches, updated to mainline semi-regularly.
Only Keystone modification is the Keystone driver, in arch/riscv/drivers/.

riscv-pk
--------

The proxy-kernel for machine-mode riscv. Trusted core component of
Keystone, and includes the security monitor.

riscv-qemu
----------

Qemu with riscv patches and minor modifications to better support PMP
and Keystone needs for emulation. Our default testing platform.

sdk
---

Tools, libraries, and tests for building applications on Keystone.
Includes the runtime submodule that runs in S-mode in enclaves.


keystone-demo
-------------

A complete demo application using Keystone. Hosts an enclave that
performs computation over data provided by a remote client using
secure channels.

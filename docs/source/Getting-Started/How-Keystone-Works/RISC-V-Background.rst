RISC-V Background
===================================

Keystone Enclave is an enclave for RISC-V processors.
`RISC-V <https://riscv.org>`_ is an open and free instruction set architecture (ISA), which allows anyone to use, modify, and extend.

RISC-V presents us with a number of benefits besides just being open-source:

* RISC-V has added security-oriented primitives (notably Physical Memory Protection) that enable efficient isolation.

* RISC-V is an evolving and community driven ISA. Keystone can explore the design space of useful
  security features, and feed good ideas back into the standards themselves.

* RISC-V has a constantly expanding world of open-source cores and products. This gives Keystone a
  wide variety of potential platforms and uses it can adapt to.

In addition, RISC-V software privilege specification fits well for Keystone, which requires transparent and agile patch on the trusted computing base (TCB).
To understand why this is true, see the next section explaining why using M-mode as a TCB is a great idea.

.. note::

  Keystone is based on the lastest stable ISA specification at the time of writing (User 2.2 and Priv. 1.10).
  This document only contains high-level ideas of several components in the privileged ISA.
  For more details, please refer to `RISC-V Spec Documentations <https://riscv.org/specifications/>`_.


ISA Compatibility
----------------------------------

Currently, Keystone is compatible with a specific subset of RISC-V ISA as follows:

* Keystone requires all of three privilege modes (M/S/U) to support dynamic user-level isolation in Linux machines.
* rv64gc-lp64d (Sv39 virutal addressing mode)
* rv32gc-ilp32d (Sv32 virtual addressing mode)

Recently, Keystone enclaves in M/U-only machines are prototyped in FreeRTOS.
We are planning to upstream the version in the near future.

RISC-V Privilieged ISA
-----------------------------------

RISC-V has three software privilege levels (in increasing order of capability): user-mode (U-mode), supervisor mode (S-mode), and machine mode (M-mode).
The processor can run in only one of the privilege modes at a time.

.. note::

  RISC-V also defines hypervisor priviliege mode (H-mode), but the spec of H-mode is not included in the stable revision (RISC-V Priv. v1.10).

Privilege level defines what the running software can do during its execution.
Common usage of each privilege level is as follows:

* U-mode: user processes
* S-mode: kernel (including kernel modules and device drivers), hypervisor
* M-mode: bootloader, firmware

Some embedded devices may have only M-mode or only M/U modes.

M-mode is the highest privilege mode and controls all physical resources and interrupts.
M-mode is analogous to microcode in complex instruction set computer (CISC) ISAs such as x86,
in that it is not interruptible and free from interference of lower modes.

Keystone uses M-mode for running the *security monitor (SM)*, the trusted computing base (TCB) of the system.

There are several benefits for using an M-mode software as the TCB:

**Programmability**:
Unlike microcode, we can build M-mode software in existing programming languages (i.e., C) and toolchains (i.e., gcc).

**Agile Patching**:
Since the SM is entirely software, it is possible to patch bugs or vulnerabilities without involving hardware-specific updates.

**Verifiability**:
In general, software is easier to formally verify than hardware.

Physical Memory Protection (PMP)
-----------------------------------

The RISC-V Priv 1.10 standard introduced Physical memory protection
(PMP): a strong standard primitive that allows M-mode to control
physical memory access from lower privileges (U-/S-modes). Keystone
requires PMP to implement memory isolation of enclaves.

PMP is controlled by a set of control and status registers (CSR) to
restrict physical memory access to the U-mode and S-mode, and can only
be configured by M-mode. The number of PMP entries varies depending on
the platform design. For example, RocketChip has 8 PMP entries per
core by default, where QEMU virt machine has 16 entries. Each PMP
entry is defined by 1 or more PMP CSRs depending on the configuration
mode.

As PMP operates on physical addresses, the configuration of virtual
addresses can remain a capability of S-mode without compromising
security. While the actual hardware implementation of PMP may vary
among processors, the basic guarantees are part of the standard.


:doc:`/Security-Monitor/index` relies on PMP for implementing memory isolation.

Interrupts and Exceptions
----------------------------------

By default, M-mode is the first receiver of any interrupts or
exceptions (i.e., traps) in the system.  Thus, M-mode has complete
authority over CPU scheduling and configuration, but may delegate this
control as needed to S-mode.

Optionally, M-mode can disable or enable each of the interrupts by using ``mie`` CSR.
M-mode can also delegate traps to S-mode by setting bits of the trap delegation registers (i.e., ``mideleg``
and ``medeleg``).
Trap delegation enables skipping M-mode handler so that S-mode can quickly handle frequent traps
such as page faults, system calls (environment call), and so on.

Virtual Address Translation
----------------------------------

U- and S-modes accesses memory via virtual addresses, while M-mode operates exclusively on physical addresses.

Virtual address translation in RISC-V is performed by a memory
management unit (MMU) consisting of two hardware components: a page
table walker (PTW) and a translation look-aside buffer (TLB).
RISC-V uses multi-level page table, where number of pages and the size of a page depends on the
addressing mode.
The ``satp`` CSR determines which addressing mode MMU should use and which physical page contains the
root page table for beginning page table walks.

Although S-mode (the OS) may change ``satp`` arbitrarily, Keystone
enclaves are not susceptible to any attacks based on altering the page
table.  Enclaves are protected from external access by the OS with
physical address restrictions via PMP, and have their own protected
page tables that cannot be modified by the OS.

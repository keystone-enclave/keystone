RISC-V Background
===================================

Keystone Enclave is an enclave for RISC-V processors.
`RISC-V <https://riscv.org>`_ is an open, free instruction set architecture (ISA), which allows anyone to use, modify, and extend.
We choose RISC-V because of several reasons.
First, RISC-V ISA is completely free to modify or extend, so you can add existing extensions or define your own extension.
Yet, Keystone itself does not require any ISA modification since it relies on only standard primitives.
Second, RISC-V has a lot of open-source processors (or SoCs) that you can experiment on.
`RocketChip <https://github.com/freechipsproject/rocket-chip>`_, 
`Berkeley Out-of-Order Machine (BOOM) <https://github.com/riscv-boom/riscv-boom>`_, 
or `MIT RISCY <https://github.com/csail-csg/riscy>`_ are some of the examples, and we expect more will be available as the community grows.
Third, RISC-V software privilege specification fits well for Keystone, which requires transparent and agile patch on the trusted computing base (TCB).
To understand why this is true, see the next section explaining why using M-mode as a TCB is a great idea.

.. note::

  Keystone is based on the lastest stable ISA specification at the time of writing (User 2.2 and Priv. 1.10).
  This document only contains high-level ideas of several components in the privileged ISA.
  For more details, please refer to `RISC-V Spec Documentations <https://riscv.org/specifications/>`_.

RISC-V Privilieged ISA
-----------------------------------

RISC-V has three software privilege levels: user-mode (U-mode), supervisor mode (S-mode), and machine mode (M-mode). 
The processor can run in only one of the privilege modes at a time.

.. note::

  RISC-V also has hypervisor priviliege mode (H-mode), but the spec of H-mode is not included in the stable revision (RISC-V Priv. v1.10). 

Privilege level defines what the running software can do during its execution. 
Common usage of each privilege level is as follows:

* U-mode: user processes
* S-mode: kernel (including kernel modules and device drivers), hypervisor
* M-mode: bootloader, firmware

Some embedded devices may not have all of three privilege levels. 
That being said, M-mode is only the mendatory privilege level where U- and S-modes can be optionally supported by the processor.

M-mode is the highest privilege mode which can control all the physical resources and events.
M-mode is somewhat similar to microcode in complex instruction set computer (CISC) ISAs such as x86,
in that it is not interruptible and free from interference of lower modes.
However, M-mode still shares the same set of instructions specified in the ISA.
Because of these properties, we can use M-mode for running the *security monitor (SM)*, the trusted computing base (TCB) of the system.

There are several benefits for using an M-mode software as the TCB:

**Programmability**:
Unlike microcode, we can build M-mode software with the existing programming language (i.e., C) and toolchain (i.e., gcc).

**Agile Patch**:
Since the SM is entirely software, it is much more easier to patch bugs or vulnerabilities without involving hardware-specific updates.

**Verifiability**:
In general, software is easier to formally verify then hardware.

Physical Memory Protection (PMP)
-----------------------------------

Physical memory protection (PMP) which was introduced in RISC-V Priv. 1.10, 
is a strong standard primitive that allows M-mode to control physical memory access from lower privileges (U-/S-modes).
Keystone utilizes PMP to implement the memory isolation of enclaves.

PMP is a feature that allows M-mode to use a set of control and status registers (CSR) to restrict physical memory access to the U-mode and S-mode.
Desirably it is implemented by rejecting entry of the translation look-aside buffer (TLB) because
U-mode and S-mode may access memory based on virtual address.
An actual hardware implementation of PMP may vary among processors.

PMP is configured by several PMP entries. 
The number of PMP entires vary depending on the platform. For example, RocketChip has 8 PMP entries
per core by default, where QEMU virt machine has 16 entries.
PMP entries are defined by a set of PMP CSRs.
Only M-mode has write permission to these CSRs, thus M-mode can effectively control the physical access of other privilege
modes.

:doc:`Keystone-Security-Monitor` relies on PMP for implementing memory isolation.

Interrupts and Exceptions
----------------------------------

By default, M-mode is the first receiver of any interrupts or exceptions (i.e., traps) in the system.
This essentially allows M-mode to fully control the hardware.
In other words, S-mode or U-mode cannot interfere CPU when they're not allowed to.
This is a very important property that any hardware running Keystone security monitor should correclty support.

Optionally, M-mode can disable or enable each of the interrupt by using ``mie`` CSR.
M-mode can also delegeate some of the traps by setting bits of the trap delegation registers (i.e., ``mideleg``
and ``medeleg``).
Trap delegation enables skipping M-mode handler so that S-mode can quickly handle frequent traps
such as page faults, system calls (environment call), and so on.

Virtual Address Translation
----------------------------------

U- and S-modes accesses memory via virtual addresses. 
A virtual address is translated into a physical address by memory management unit (MMU) in the
processor.
MMU consists of two key hardware components: a page table walker (PTW) and a translation
look-aside buffer (TLB).
MMU translates a virtual address based on the *page table*.

RISC-V uses multi-level page table, where number of pages and the size of a page depends on the
addressing mode.
``satp`` CSR determines which addressing mode MMU should use and which physical page contains the
root page table.

.. attention::

  Keystone now only supports RV64 with Sv39 addressing mode, which translates 39-bit virtual addresses into
  50-bit physical addresses based on a 3-level page table.

Although the OS has an authority to change ``satp`` and to construct a page table, Keystone enclaves are not
susceptible to any attacks based on altering the page table.
First, PMP asserts access permissions based on the **physical address**, which prevents the OS from
accessing the protected address range, even if the translation succeeded.
Second, in Keystone, each enclave has its own page table within its memory, which makes the enclave page
table completely oblivious from the OS.



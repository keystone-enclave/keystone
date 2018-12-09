RISC-V Background
===================================

Keystone Enclave is an enclave for RISC-V processors.
RISC-V is an open, free instruction set architecture (ISA), which allows anyone to use, modify, and extend.
We choose RISC-V because of several reasons.
First, RISC-V is the best ISA you can do a lot of experiments.
Since it's completely free to modify the ISA itself, you can define your own instructions, add non-standard extension, and so on.
Yet, Keystone itself does not require any ISA modification since it relies on only standard primitives.
Second, RISC-V has a lot of open-source processors (or SoCs) that you can integrate Keystone.
RocketChip, Berkeley Out-of-Order Machine (BOOM), or MIT RISCY are the examples, and more will soon be available as the community grows.
Third, RISC-V software privilege specification fits well for Keystone, which requires transparent & agile patch on the trusted computing base (TCB).
To understand why this is true, see the next section.

.. note::

  Keystone is based on the lastest stable ISA specification at the time of writing (User 2.2 and Priv. 1.10).
  For more information, please refer to `RISC-V Spec Documentations <https://riscv.org/specifications/>`_.

RISC-V Privilieged ISA
-----------------------------------

RISC-V has three software privilege levels: user-mode (U-mode), supervisor mode (S-mode), and machine mode (M-mode). 
At a point of time, the processor can run in only one of the privilege modes.

.. note::

  RISC-V also has hypervisor priviliege mode (H-mode), but the spec of H-mode is not included in the stable revision (RISC-V Priv. v1.10). 

Privilege level defines what instructions can do during its execution. 
Common usage of each privilege level is as follows:

* U-mode: user processes
* S-mode: kernel, kernel modules, device drivers, hypervisor
* M-mode: bootloader, firmware

Some embedded devices may not have all of three privilege levels. 
That being said, M-mode is only the mendatory privilege level where U- and S-modes can be optionally supported by the processor.

M-mode is the highest privilege mode which can control all the physical resources and events.
M-mode is somewhat similar to microcode in complex instruction set computer (CISC) ISAs such as x86,
in that it is not interruptible and free from interference of lower modes.
However, M-mode still shares the same set of instructions specified in the ISA.
Because of these property, we can use M-mode for running the *security monitor (SM)*, the trusted computing base (TCB) of the system.

There are several benefits for using an M-mode software as the TCB:

* Programmability: Unlike microcode, we can build M-mode software with the existing programming language (i.e., C) and toolchain (i.e., gcc).
* Agile Patch: Since the SM is entirely software, it is much more easier to patch bugs or vulnerabilities without involving hardware-specific updates.
* Verifiability: In general, software is easier to formally verify then hardware.



Physical Memory Protection (PMP)
-----------------------------------

Physical memory protection (PMP) which was introduced in RISC-V Priv. 1.10, 
is a strong standard primitive that allows M-mode to control physical memory access from lower privileges (U-/S-modes).
Keystone utilizes PMP to implement the memory isolation of enclaves.

PMP is a feature that allows M-mode to use a set of control and status registers (CSR) to restrict physical memory access to the U-mode and S-mode.
Desirably it is implemented by rejecting entry of the translation look-aside buffer (TLB) because
U-mode and S-mode may access memory based on virtual address.
An actual hardware implementation of PMP may vary among processors.



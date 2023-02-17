Keystone Security Monitor
====================================

The security monitor (SM) is trusted software that runs in M-mode, and forms the trusted computing
base (TCB) in Keystone system.
The SM is the most important software in Keystone as it provides most of the security guarantees.
The SM provides the following functionality:

1. Memory Isolation using RISC-V PMP
2. Remote Attestation (Signatures and measurement)
3. Enclave Thread Management
4. System PMP Synchronization
5. Side-Channel Defenses

The security monitor must be verified by the *root of trust* in hardware before being trusted.
After the root of trust measures the SM it generates a keypair for remote attestation, signs the public key, and then continues booting.
See `Chain of Trust <#>`_ for details.

The OS and enclaves may call SM functions using the supervisor binary interface (SBI),
For detailed specification, see `SBI Documentation <https://github.com/keystone-enclave/keystone/blob/master/sm/spec/v1.0.md>`_.

Specific platforms
------------------
.. toctree::
   :maxdepth: 1

   FU540

PMP Internals
-------------

PMP allows for N (platform defined) entries, which are configured
using a config (cfg) and address (addr) register each. There are
further details available in the RISC-V `specification
<https://content.riscv.org/wp-content/uploads/2017/05/riscv-privileged-v1.10.pdf>`_
section 3.6.

In Keystone we refer to the section of memory that an enclave uses as
a *region* and each region is defined by a PMP entry.

Memory Isolation using RISC-V PMP
-------------------------------------

The SM currently consumes *two* PMP entries for its own uses (i.e., security monitor memory and untrusted memory).
Each of the remaining PMP entries may be consumed by one active enclave each.

To understand how the SM utilizes PMP, we need to know several properties of RISC-V PMP:

1. **Prioritized by Index**

	PMP entries are statically priortized by their index, with a check stopping at the highest priority matching.
        Indices run from 0..N (where N is platform defined), with 0 having the highest priority, and N having the lowest.
	Thus, the access permissions to a physical address should be of the lowest-index PMP entry among the matched ones.

2. **Default Deny**

	If no PMP entry matches with an address, the memory access will be rejected by default.

3. **Dynamically Configurable by M-mode**

	M-mode can write to PMP CSRs during runtime to define PMP entries dynamically.
	A TLB flush is required after reconfiguration.

4. **Addressing Modes**

	PMP supports two addressing modes: naturally-aligned power of two (NAPOT), or top-of-range (TOR).
	TOR consumes adjacent PMP entry's address register for the bound address to allow shared boundaries efficiently.
        Ex: Two ranges of 0-2048 and 2048-4096 uses 3 entries, 0, 2048, 4096.


At the very beginning of the boot process, physical memory is not accessible by U- or S-modes
because of property 2 as shown in the diagram below.

.. code-block:: text

  For ease of understanding, we denote PMP entries as pmp[i] where i is an index.
  There are N PMP entries.

  -: inaccessible (NO_PERM), =: accessible (ALL_PERM)

  pmp[1:N]     |                                        | : undefined
  net result   |----------------------------------------|

The SM sets the first PMP entry to cover the address range containing itself (``0x80000000``-``0x80200000``), and sets all permission bits to 0.
The SM then sets the last PMP entry to cover the entire DRAM and sets all permission bits to 1, allowing the OS to access the rest of the memory and start booting.
Because of the property 1, the net result will be as follows:

.. code-block:: text

  -: inaccessible (NO_PERM), =: accessible (ALL_PERM)

  pmp[0]       |-------|                                | : SM memory
  pmp[others]  |                                        | : undefined
  pmp[N]       |========================================| : OS memory
  net result   |-------|================================|

When the SM creates an enclave, it will assign a PMP entry to the enclave to protect its memory from other U-/S-mode software.

.. code-block:: text

  -: inaccessible (NO_PERM), =: accessible (ALL_PERM)

  pmp[0]       |-------|                                | : SM memory
  pmp[1]       |              |---------|               | : enclave memory
  pmp[others]  |                                        | : undefined
  pmp[N]       |========================================| : OS memory
  net result   |-------|======|---------|===============|

When the SM enters the enclave (executes the enclave), it flips the permission bits of the enclave's PMP entry and the last PMP entry.
In addition, Keystone allows the OS to allocate an additional contiguous memory region in the OS memory space so that the enclave can use it as a communication buffer.
The buffer is called untrusted shared buffer.

.. code-block:: text

  -: inaccessible (NO_PERM), =: accessible (ALL_PERM)

  pmp[0]       |-------|                                | : SM memory
  pmp[1]       |              |=========|               | : enclave memory
  pmp[others]  |                                        | : undefined
  pmp[N]       |                                |==|    | : untrusted shared buffer
  net result   |-------|------|=========|-------|==|----|

When the SM exits the enclave, it simply flips the permission bits back.
When the SM destroys an enclave, it clears the PMP entry so that other enclaves can use it.


Remote Attestation
-------------------------------------

The goal of remote attestation is to prove to a remote client that the enclave contains the program expected, and is running on hardware that is trusted.
In Keystone this involves a few cryptographic operations including ECDSA signature and SHA-3 hash.

For more detail about the remote attestation process, see :doc:`Remote Attestation </Keystone-Applications/Attestation>`.

Enclave Context Management
-------------------------------------

The Keystone SM manages enclave context (e.g., general purpose registers, trap vector, etc) and the status of each hardware thread.
When an enclave enters and exits, the SM performs the following steps to switch between trusted and untrusted contexts:

#. Update thread status
#. Save the previous context
#. Restore the next context
#. Flip PMP permissions
#. Flush TLB


Building platform specific modules
----------------------------------

You can re-configure and re-build the ``bbl`` in the build directory:

::

	../configure \
		--enable-sm \
		--with-target-platform=<platform> \
		--host=riscv64-unknown-linux-gnu \
		--with-payload=<path to vmlinux>

	make


Each platform directory contains a top-level C file that handles the module.

For example, ``--with-target-platform=fu540`` will include ``sm/platform/fu540/fu540.c`` in the SM.
Each of the directories contains the hooks for each platform.

The platform directory ``sm/platform`` contains all current platform specific supports.

Currently that is:
``default`` and ``fu540`` directories.


System PMP Synchronization
-------------------------------------

.. note::

  System PMP is not supported yet.

Side-Channel Defense
-------------------------------------

The :doc:`FU540<FU540>` module supports cache side-channel defenses.

Current Enclave Lifecycle
-------------------------------------

.. graphviz:: enclave_lifecycle.dot

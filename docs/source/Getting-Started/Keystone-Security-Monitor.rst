Keystone Security Monitor
====================================

The security monitor (SM) is a trusted software that runs in M-mode, acting as a trusted computing
base (TCB) in Keystone system.
SM is the most important software in Keystone system for the security guarantees.
SM provides the following functionalities:

1. Memory Isolation using RISC-V PMP
2. Remote Attestation
3. Enclave Thread Management
4. System PMP Synchronization
5. Side-Channel Defense

.. attention::

  Version 0.1 only implements 1, 2, and 3, supporting single-threaded enclave.

The security monitor must be verified by the *root of trust* in hardware before being trusted.
After the root of trust verifies SM, it starts booting with a keypair for remote attestation, 
where the public key was signed by the root of trust.
See `Chain of Trust <#>`_ for how it works.

The OS or the enclaves can call SM functions using supervisor binary interface (SBI), 
which is implemented by environment call (i.e., ``ecall`` instruction).
See `SBI Documentation <#>`_ for details.

Memory Isolation using RISC-V PMP
-------------------------------------

SM consumes *two* PMP entries for special purposes (i.e., security monitor memory and untrusted memory).
Each of the remaining PMP entries is consumed by one active enclave.

To understand how SM utilizes PMP, we need to know several properties of RISC-V PMP:

1. **Prioritized by Index**
	
	PMP entries are statically priortized by their index.
	A lower index entry has higher matching priority (e.g., PMP entry 1 has higher priority than PMP entry 2).
	Thus, the access permissions to a physical address should be of the lowest-index PMP entry among the matched ones.

2. **Whitelist-based Rule**

	If no PMP entry matches with an address, the memory access will be rejected by default.

3. **Dynamically Configurable by M-mode**

	M-mode can write to PMP CSRs dynamically to define PMP entries. 
	TLB flush is required after reconfiguration.

4. **Addressing Modes**

	PMP supports two addressing modes: naturally-aligned power of two (NAPOT), or top-of-range (TOR).
	TOR consumes adjacent PMP entry's address register for the bound address.

.. note::
  
  Keystone SM only uses NAPOT (> 4KB), because it relies on Linux buddy allocator to get a contiguous physical memory region for each enclave.
  TOR requires PMP regions to be adjacent each other, which is not guaranteed by the buddy allocator.

At the very beginning of the boot process, physical memory is not accessible by U- or S-modes
because of the property 2 as shown in the diagram below.

.. code-block:: text 
  
  For ease of understanding, we denote PMP entries as pmp[i] where i is an index. 
  There are N PMP registers.
  
  -: inaccessible (NO_PERM), =: accessible (ALL_PERM)
  
  pmp[1:N]     |                                        | : undefined
  net result   |----------------------------------------| 

SM sets the first PMP entry to cover the address range containing itself (``0x80000000``-``0x80200000``), and sets all permission bits to 0.
SM then sets the last PMP entry to cover the entire DRAM and sets all permission bits to 1, so that the OS can access the rest of the memory and start booting.
Because of the property 1, the net result will be as follows:

.. code-block:: text

  -: inaccessible (NO_PERM), =: accessible (ALL_PERM)
  
  pmp[0]       |-------|                                | : SM memory
  pmp[others]  |                                        | : undefined
  pmp[N]       |========================================| : OS memory
  net result   |-------|================================| 

When SM creates an enclave, it will assign a PMP entry to the enclave to protect its memory from other U-/S-mode software.

.. code-block:: text
  
  -: inaccessible (NO_PERM), =: accessible (ALL_PERM)
  
  pmp[0]       |-------|                                | : SM memory
  pmp[1]       |              |---------|               | : enclave memory
  pmp[others]  |                                        | : undefined
  pmp[N]       |========================================| : OS memory
  net result   |-------|======|---------|===============| 

When SM enters the enclave (executes the enclave), it flips the permission bits of the enclave's PMP entry and the last PMP entry.
In addition, Keystone allows the OS to allocate an additional contiguous memory in the OS memory space so that the enclave can use it as a communication buffer.
The buffer is called untrusted shared buffer.

.. code-block:: text
  
  -: inaccessible (NO_PERM), =: accessible (ALL_PERM)
  
  pmp[0]       |-------|                                | : SM memory
  pmp[1]       |              |=========|               | : enclave memory
  pmp[others]  |                                        | : undefined
  pmp[N]       |                                |==|    | : untrusted shared buffer
  net result   |-------|------|=========|-------|==|----| 

When SM exits the enclave, SM simply flips the permission bits back.
When SM destroys an enclave, it unsets the PMP entry so that other enclaves can use it.


Remote Attestation
-------------------------------------

The goal of attestation is to prove that the enclave contains the right program that you expect, and is running on a real hardware that you trust.
This involves a few cryptographic operations including ECDSA signature and SHA-3 hash.

For more detail about the remote attestation process, see :doc:`Remote Attestation <../Keystone-Applications/Attestation>`.

Enclave Context Management
-------------------------------------

Keystone SM manages enclave context (e.g., general purpose registers, trap vector, etc) and the status of each hardware thread.
When enclave enters and exits, SM does the following steps to switch between trusted and untrusted contexts

#. Update thread status
#. Save the previous context 
#. Restore the next context
#. Flip PMP permissions
#. Flush TLB

System PMP Synchronization
-------------------------------------

.. note::
  
  System PMP is not supported yet

Side-Channel Defense
-------------------------------------

.. note::
  
  Side channel defense is not implemented yet

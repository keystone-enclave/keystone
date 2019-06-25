Keystone Basics
=========================================

Keystone is an open framework for architecting trusted execution environments (TEEs) based on hardware enclaves.
Keystone aims to provide an easy way of architecting and building a customized TEE for an arbitrary RISC-V platform with a simple workflow.
This allows the TEE to be highly optimized for better performance, smaller trusted computing base (TCB),
and programmability with a given workload and a threat model.

Overview
-------------------------------

A Keystone-capable system consists of several components in different privilege modes.

.. figure:: /_static/images/keystone_overview.png

**Trusted Hardware** is a CPU package built by a trustworthy vendor, and must contain Keystone-compatible *standard RISC-V cores* and *root of trust*.
The hardware may also contain optional features such as cache partitioning, memory encryption, cryptographically-secure source of randomness, etc.
The Security Monitor requires platform specific plug-ins for optional feature support.

**Security Monitor (SM)** is M-mode software with small TCB.
The SM provides an interface for managing the lifecycle of enclave as well as for utilizing platform-specific features.
The SM enforces most of Keystone's security guarantees since it manages the isolation boundary between the enclaves and the untrusted OS.

**Enclaves** are environments isolated from the untrusted OS and other enclaves. Each enclave is given a private physical memory region which is accessible by only the enclave and SM.
Each enclave consists of a user-level enclave application *eapp* and a supervisor-level *runtime*.

**Enclave Application (eapp)** is the user-level application that executes in the enclave. One can build a custom eapp from the scratch, or just run an existing RISC-V executable in Keystone.

**Runtime** is S-mode software which implements functionality such as system calls, trap handling, virtual memory management and so on.

Keystone Workflow
-------------------------------

.. figure:: /_static/images/keystone_workflow.png

Keystone is a framework wherein both *platform provider* and *enclave developer* customize elements of the system to their needs.
This involves two separate workflows: one for each of the platform provider and the enclave developer. 

Provisioning the Security Monitor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A **platform provider** the party who provides a machine which can run
Keystone enclaves. In the *provisioning* stage, the platform provider
compiles the security monitor and deploys it to the machine.

The platform provider 
configures(❶), builds(❷), and then deploys(❸) the security monitor with target hardware.

See :doc:`/Getting-Started/Tutorials/Security-Monitor-Platform-Build` to learn how to build and
deploy the security monitor.

Developing the Enclave
~~~~~~~~~~~~~~~~~~~~~~

A **enclave developer** develops an enclave with the Keystone SDK.
The developer builds the eapp, the host, and the runtime binaries, and
(optionally) packages them together into a single file (❹).

The enclave components are then sent to the remote machine running
Keystone and deployed (❺, ❻, and ❼).

See :doc:`/Getting-Started/Tutorials/Build-Enclave-App-Hello-World` to learn how to build an
enclave.

Remote Attestation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The enclave is challenged at the beginning of the execution by a trusted remote verifier (❽).
The remote verifier could be the developer herself, a trusted party, or a person who wants to run an
enclave built by others.
The remote attestation uses cryptographically-secure signatures and measurements to check if a
legitimate security monitor was deployed on a trusted hardware and if the enclave is initialized as expected.

See :doc:`/Getting-Started/Tutorials/Remote-Attestation` to learn how to implement remote
attestation.

Enclave Lifecycle
-------------------------------

.. figure:: /_static/images/enclave_lifecycle.png


Creation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An enclave starts with a contiguous range of physical memory called enclave private memory (EPM).
The untrusted host first allocates the EPM, and initializes it with the enclave's page table (PT),
the runtime (RT), and the eapp.
Any residual EPM will be consumed as *free memory*.
The host calls the SM to create an enclave using ``create_enclave`` SBI call.
The SM protects and isolates the EPM using a free PMP entry.
The PMP status is propagated through all cores in the system so that the EPM is protected from any
cores.
As soon as the enclave is created, the SM measures and verifies the initial state of the enclave.

Execution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The host can enter the enclave using ``run_enclave`` SBI call on one of the cores.
The SM releases the PMP permission to the core, and the core starts to execute the enclave.
The runtime can exit the enclave any time through ``stop_enclave`` or ``exit_enclave`` SBI call.
When a core exits the enclave, it restores the PMP entry.
The core can resume the enclave with ``resume_enclave`` SBI call.

The host can allocate more memory to the enclave using ``extend_enclave`` SBI call.
This feature is currently under testing, and will be available in the future versions.

Destruction
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The host can use ``destroy_enclave`` SBI call to destroy the enclave.
The SM cleans the EPM and release the PMP entry.
The host reclaims the EPM and deallocates the memory.

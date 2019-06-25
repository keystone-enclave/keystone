Keystone Basics
=========================================

Keystone is an open framework for architecting trusted execution environments (TEEs) based on hardware enclaves.
Keystone aims to provide an easy way of architecting and building a customized TEE for an arbitrary RISC-V platform with a simple workflow.
This allows the TEE to be higly optimized for better performance, smaller trusted code base (TCB),
and programmability with a given workload and a threat model.

Overview
-------------------------------

A Keystone-capable system consists of several components in different privilege modes.

.. figure:: /_static/images/keystone_overview.png

**Trusted Hardware** is a CPU package built by a trustworthy vendor, and must contain Keystone-compatible *standard RISC-V cores* and *root of trust*.
The hardware may also contain optional features such as cache partitioning or cryptographically-secure source of randomness.
Keystone security monitor must be built for each of the platform so that it can utilize these additional features for the TEE.

**Security Monitor (SM)** is an M-mode software with small TCB.
SM provides an interface for managing the lifecycle of enclave, as well as for utilizing platform-specific features.
The SM is crucial for the security guarantees of the TEE since it enforces the isolation boundary between the enclaves and the untrusted OS.

**Enclaves** are trusted execution environments isolated from the untrusted OS or the other enclaves. Each enclave is given a private physical memory region which is exclusively accessible by the enclave itself. Each enclave consists of an user-level enclave application (eapp) and a supervisor-level *runtime*.

**Enclave Application (eapp)** is the user-level application that will run in the enclave. One can build a custom eapp from the scratch, or just run an existing RISC-V executable in Keystone.

**Runtime** is a S-mode software, which selectively serves the untrusted kernel's functionality such as system call, trap handling, virtual memory management and so on.

Keystone Workflow
-------------------------------

.. figure:: /_static/images/keystone_workflow.png

Keystone is a framework wherein both *platform provider* and *enclave developer* can customize their enclaves depending on their needs.
This involves two separate workflow from each of the platform provider and the enclave developer. 

Provisioning the Security Monitor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A **platform provider** is a party who provides a machine which can run Keystone enclaves. In
*provisioning* stage, the platform provider compiles the security monitor and deploys it to the machine. 

The platform provider 
configures and builds the security monitor with target hardware
(❶ and ❷).
The platform provider then deploys the security monitor to the machine (❸).

See :doc:`/Getting-Started/Tutorials/Security-Monitor-Platform-Build` to learn how to build and
deploy the security monitor.

Developing the Enclave
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

A **enclave developer** develops an enclave with Keystone SDK.
The developer builds the eapp, the host, and the runtime binaries, and package them together into
a
single file (❹).

The enclave is sent to the untrusted remote machine and deployed (❺, ❻, and ❼).

See :doc:`/Getting-Started/Tutorials/Build-Enclave-App-Hello-World` to learn how to build an
enclave.

Remote Attestation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Keystone supports basic remote attestation workflows. In these
setups the enclave may present proof of its initial state and
measurement by the SM to a remote party for validation (❽).

See :doc:`/Getting-Started/Tutorials/Remote-Attestation` to learn how to implement a simple remote
attestation.

Enclave Lifecycle
-------------------------------

.. figure:: /_static/images/enclave_lifecycle.png


Creation
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

An enclave starts with a contiguous range of physical memory called enclave private memory (EPM).
The untrusted host first allocates the EPM, and initializes it with the enclave's page table (PT),
the runtime (RT), and the eapp.
Once the host calls the SM to create an enclave,
The SM walls off the EPM using a PMP entry.
The PMP status is propagated through all cores in the system so that the EPM is protected from any
cores.
As soon as the enclave is created, the SM measures and verifies the initial state of the enclave.

Execution
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The host asks the SM to enter the enclave on one of the cores.
The SM releases the PMP permission to the core, and the core starts to execute the enclave.
The runtime can exit or re-enter the enclave any time.
When a core exits/enters the enclave, it accordingly switches the PMP permissions to keep the
isolation.

Destruction
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The host can destroy the enclave to reclaim the memory.
The SM cleans the EPM and release the PMP entry.

SDK Overview
============

.. attention::

  Keystone SDK is work in progress, so programming enclaves requires a lot of manual efforts.
  However, it still provides enough libraries to run simple enclaves such as the Keystone Demo.

The Keystone SDK provides the tools required to build basic enclave
hosting applications (`hosts`) and enclave applications (`eapps`).

The SDK consists of 4 main components, and examples.
 - Host libraries (``src/host``)
 - Enclave application libraries (``src/app``)
 - Edge libraries (``src/edge``)
 - Attestation libraries (``src/verifier``)

Host Libraries
--------------

The host libraries provide an interface for managing enclave
applications via the ``Enclave`` class. Most of this library will work
regardless of the runtime, but is tied directly to the kernel driver
provided in the `riscv-linux` repository branches.

Eapp Libraries
--------------

The eapp libraries provide both simple enclave tools (EXIT, etc) as
well as some basic libc-style functions (malloc, string headers, etc).

Edge Libraries
--------------

The edge libraries provide features to both eapps and hosts for
managing edge calls. Edge calls are function calls that cross the
enclave-host boundary. Currently we only support calls from
enclave->host. You can emulate host->enclave calls via polling on this
interface. The edge libraries are used in many places, including the
runtime and both host and eapp libraries.

Writing A Simple Application
============================

The best way to see how to write a simple application is to look a the
``untrusted`` test eapp, and the ``keystone-demo`` repository.

A full system for using a Keystone enclave consists of possibly
writing 3 things:
- Host (userspace, outside enclave, untrusted)
- Runtime (system level, inside enclave, trusted) - most users will not modify this
- Enclave app (userspace, inside enclave, trusted)

Most users will only need to write a simple Host, and use the Eyrie
runtime. The bulk of the work is done in the enclave application, and
in the glue that holds the components together.

NOTE: Right now all "edge" components (that is, anything that handles
data and calls between enclave code and non-enclave code) is
hand-written. A major next step for the SDK is to build a set of tools
and compilers to do the majority of this code generation
automatically.

Writing a host
--------------

Most host functionality is contained in the ``Keystone::Enclave`` C++ class in
the host library. To start an enclave application, first create one::

  Keystone::Enclave enclave;
  Keystone::Params params;

  [... Optional params settings ...]

  enclave.init(PATH_TO_EAPP_ELF, PATH_TO_RUNTIME, params);

  edge_init(&enclave);

You can also set other enclave options via the params object,
including stack size and shared memory size. ``edge_init`` is an edge
wrapper function that registers edge call endpoints in the Keystone
driver.

Then you start the enclave and transfer control with::

  enclave.run()

At this point, control is transferred to the enclave runtime, and then
application (if using the default runtime). Generally the first action
after this would be to have the enclave send its attestation report to
the host. Control will be transferred back to the host code either
when the enclave exits, or when an edgecall is made.

Writing an eapp
---------------

A generic RISC-V statically compiled ELF binary will run.

As with the host, the first action should be::

  edge_init()

Which is another edge call wrapper function to register call points
and setup buffers. Once complete, the first action should be to make
an edgecall to the host to present the attestation report.

After this point, all functionality is up to the application
developer. See the `keystone_demo` repository for an example
application.

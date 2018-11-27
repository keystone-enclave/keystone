Compiling Applications
======================

Building a host and enclave application using the SDK is
straight-forward. See Keystone-Demo as an example.


Toolchain
---------

All compilation will need to be done using the riscv64- toolchain.
This is provided by the riscv-gnu-toolchain submodule, and must be built first.

Libraries
---------

Hosts and enclave applications will want to link against the edge library ``libkeystone-edge.a``

Hosts will want to link against the host library ``libkeystone-host.a``

Applications will want to link against the enclave app library ``libkeystone-app.a``

Applications
------------

Eapps need to be linked in a specific way. As this may change, please
see the ``app.lds`` linker script in the sdk/tests directory to see
the most up to date linking requirements.

Compiling Applications
======================

Building a host and enclave application using the SDK is
straight-forward. See Keystone-Demo and the
:doc:`Tutorials</Getting-Started/Tutorials/index>` as examples.

Toolchain
---------

All compilation will need to be done using the riscv64- toolchain.

Libraries
---------

Hosts and enclave applications will want to link against the edge library ``libkeystone-edge.a``

Hosts will want to link against the host library ``libkeystone-host.a``

Applications will want to link against the enclave app library ``libkeystone-app.a``

Applications
------------

eapps may be a standard unmodified statically linked binary. This
requires most options for the `eyrie` runtime to be enabled.

Otherwise, eapps need to be linked in a specific way. As this may
change, please see the ``app.lds`` linker script in the ``sdk/examples/tests``
directory to see the most up to date linking requirements.

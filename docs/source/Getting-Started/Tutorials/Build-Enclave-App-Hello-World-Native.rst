Tutorial 1: Hello, World! (Native Keystone Application)
=======================================================

This tutorial covers building a simple "hello world" eapp that uses
registered function calls between the enclave and the untrusted host
to pass text strings. We refer to this style of design as a 'native'
Keystone enclave application in contrast to hosting a standard RISC-V
binary as the eapp. You can see this other style in the next tutorial
(TODO link)

Before jumping into the tutorial, please complete :doc:`Quick Start
<../Running-Keystone-with-QEMU>`.

In this tutorial you will compile the Eyrie runtime with specific
plugin support, build a host application, and build and modify an
enclave application (eapp).

Prerequisites
-------------

You must have a completed build of the sdk, qemu, and other components
outlined in :doc:`Quick Start <../Running-Keystone-with-QEMU>`.

Set ``PATH`` to include RISC-V tools and ``KEYSTONE_SDK_DIR`` to point the
absolute path to ``sdk`` directory.

::

	export PATH=$PATH:<path to RISC-V tools>
	export KEYSTONE_SDK_DIR=<path to SDK>



First Build
-----------

All code and build tools for this tutorial are found in
``sdk/examples/hello-native``.

We will be using our standard example build script, ``vault.sh`` for
this tutorial build to emit a self-extracting archive with all
binaries needed for the enclave to run.

You can read more about this tool in TODO link.

For now, you don't need to modify ``vault.sh`` at all. Just run
``./vault.sh``, and it should build ``hello-native.ke`` and put a copy
in the buildroot overlay.

Now, running ``make`` at the top-level of the Keystone project will
rebuild the qemu image to include our ``hello-native`` packaged binary.

Once booted, insert the kernel module and run ``./hello-native.ke``,
it should exit with success, but print nothing.


Internals
---------

The enclave package consists of three parts:
 - a host application
 - a runtime binary (Eyrie)
 - an eapp

In this case, the host code is in ``sdk/examples/hello-native/host/``,
the runtime is our modular Eyrie runtime (in ``sdk/rts/eyrie/`` if you
followed the previous guides), and the eapp is in
``sdk/examples/hello-native/eapp/``.


The host contains untrusted code that will ask for an enclave to be
provisioned, use the SDK to load the runtime and eapp into the enclave
space, and communicate with the enclave after it has started.

The runtime is described in detail in TODO link.

The eapp currently contains wrapper code for communicating to the
host, and currently prints "Hello World".

Printing from the eapp
----------------------

Open ``sdk/examples/hello-native/eapp/eapp_native.c`` in your editor.

In ``main`` there is a call to an ocall wrapper function,
``ocall_print_string``. This in ``ocall_print_string`` you can see the
ocall itself, which is a special type of syscall to the Eyrie
runtime. This syscall will package and pass the arguments back to the
host application, who has a registered handler for
``OCALL_PRINT_STRING``.

Open ``sdk/examples/hello-native/host/host_native.cpp`` in your
editor.

In ``main`` we have standard enclave creation and intialization, as
well as registration of an ocall handler,
``register_call(OCALL_PRINT_STRING, print_string_wrapper);`` This
function (``print_string_wrapper``) is the host-side argument parsing
and de-serialization portion of the ocall. Once it has validated and
extracted the arguments to the function, it calls ``print_string``
which finally prints the string from the eapp.

For more details on the ocall interface and passing data between the
eapp and host, see TODO link.

Modifying the eapp print
------------------------

To update the printed message, update the message in
``eapp_native.c``, re-run the vault.sh to rebuild the packaged
enclave, then rebuild the top-level qemu image in ``keystone/`` with
``make``. If you restart qemu, you'll now have the updated
``hello_native.ke`` pacakged enclave application.

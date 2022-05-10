Tutorial 2: Hello, World! (without libc)
========================================

This tutorial covers building a simple "hello world" eapp that uses
registered function calls between the enclave and the untrusted host
to pass text strings. We refer to this style of design as a 'native'
Keystone enclave application in contrast to hosting a standard RISC-V
binary as the eapp. You can see this other style in the
:doc:`previous<Build-Enclave-App-Hello-World>` tutorial.

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
absolute path to ``/keystone/sdk/build64/`` directory.

First Build
-----------

All code and build tools for this tutorial are found in
``sdk/examples/hello-native``.

You can build the enclave package by running the following command in the build directory

::

  make hello-native-package

Copy this into the buildroot overlay directory

::

  cp examples/hello-native/hello-native.ke overlay/root

Now, running ``make image`` will
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
the runtime is our modular Eyrie runtime, and the eapp is in
``sdk/examples/hello-native/eapp/``.

The host contains untrusted code that will ask for an enclave to be
provisioned, use the SDK to load the runtime and eapp into the enclave
space, and communicate with the enclave after it has started.

The runtime is described in detail in the :doc:`Eyrie
documentation</Runtimes/Eyrie>`.

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
eapp and host, see :doc:`edge
calls</Keystone-Applications/Edge-Calls>`.

Modifying the eapp print
------------------------

To update the printed message, update the message in
``eapp_native.c``, re-run ``make hello-native-package`` to rebuild the packaged
enclave, copy the package into the overlay directory, and then do
``make image``. If you restart qemu, you'll now have the updated
``hello_native.ke`` pacakged enclave application.

Deploying Enclave
------------------------------

Boot the machine with QEMU. Login as ``root`` with the password ``sifive``.

::

	./scripts/run-qemu.sh

Insert the Keystone driver

::

	# [inside QEMU]
	insmod keystone-driver.ko

Deploy the enclave

::

	# [inside QEMU]
	./hello_native.ke

You'll see the enclave running!

::

	Verifying archive integrity... All good.
	Uncompressing Keystone Enclave Package
	Hello, world!

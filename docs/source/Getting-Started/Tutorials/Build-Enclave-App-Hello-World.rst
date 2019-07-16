Tutorial 1: Hello, World!
======================================

This tutorial explains how to build and run a simple "hello world" enclave.
An enclave consists of an eapp and a runtime, but also needs the host that initializes and launches
the enclave.
Thus, each enclave source tree contains at least the host and eapp.

Before jumping into the tutorial, please complete :doc:`Quick Start
<../Running-Keystone-with-QEMU>`.


Prerequisite
------------------------------

The Eyrie runtime allows the enclave to be statically linked with
libc, and will then support a few standard functions such as
``printf``. This is not a secure I/O interface, but is useful for
demos and benchmarking.

Set ``PATH`` to include RISC-V tools and ``KEYSTONE_SDK_DIR`` to point the
absolute path to ``sdk`` directory.

::

	export PATH=$PATH:<path to RISC-V tools>
	export KEYSTONE_SDK_DIR=<path to SDK>

Let's take a look at the example provided in `Keystone SDK
<https://github.com/keystone-enclave/keystone-sdk>`_.

::

	ls sdk/examples/hello

You can find two directories and a build script called ``vault.sh``

vault.sh
------------------------------

``vault.sh`` is a sample script that builds the enclave. See full
documentation at :doc:`vault.sh</Building-Components/Vault>`.


::

	sdk/examples/hello/vault.sh

To build the enclave application package, run::

  ./vault.sh

Enclave Application: hello.c
------------------------------

Open ``hello.c`` file in ``sdk/exmamples/hello/eapp/``. This is the source code of the enclave
application.

.. code-block:: c

	#include <stdio.h>

	int main()
	{
	  printf("hello, world!\n");
	  return 0;
	}

This is the standard C program that we will run isolated in an enclave.

Host Application: host.cpp
------------------------------

Open ``host.cpp`` in ``sdk/examples/hello/host/``. This is the source code of the host application.

.. code-block:: cpp

	#include "keystone.h"
	#include "edge_call.h"
	int main(int argc, char** argv)
	{
	  Keystone enclave;
	  Params params;

	  params.setFreeMemSize(1024*1024);
	  params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 1024*1024);

	  enclave.init(argv[1], argv[2], params);

	  enclave.registerOcallDispatch(incoming_call_dispatch);
	  edge_call_init_internals((uintptr_t) enclave.getSharedBuffer(),
	      enclave.getSharedBufferSize());

	  enclave.run();

	  return 0;
	}

``keystone.h`` contains ``Keystone`` class which has several member functions to control the
enclave.

Following code initializes the enclave memory with the eapp/runtime.

.. code-block:: cpp

	Keystone enclave;
	Params params;
	enclave.init(<eapp binary>, <runtime binary>, params);


``Params`` class is defined in ``sdk/lib/host/include/params.h``, and contains enclave paraeters
such as the size of free memory and the address/size of the untrusted shared buffer.
These parameters can be configured by following lines:

.. code-block:: cpp

	params.setFreeMemSize(1024*1024);
	params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 1024*1024);

In order to handle the edge calls (including system calls), the enclave must register the edge call
handler and initialize the buffer addresses. This is done as following:

.. code-block:: cpp

	enclave.registerOcallDispatch(incoming_call_dispatch);
	edge_call_init_internals((uintptr_t) enclave.getSharedBuffer(),
	  enclave.getSharedBufferSize());

Finally, the host launches the enclave by

.. code-block:: cpp

	enclave.run();

Enclave Package
------------------------------

``vault.sh`` also contains packaging commands using ``makeself``.
``makeself`` generates a self-extracting archive with a start-up command.
All files included in ``$PACKAGE_FILES`` are copied into a directory and archived with ``makeself``.
The final output is ``hello.ke`` which is an executable file for our enclave.

Since we set ``$OUTPUT_DIR`` to buildroot overlay
directory ``$KEYSTONE_SDK_DIR/../buildroot_overlay/root/$NAME``,
running ``make`` in the top-level directory (``keystone``) will generate the buildroot disk image
containing the outputs.

::

	# go to top-level keystone directory
	make

Deploying Enclave
------------------------------

Boot the machine with QEMU.

::

	./scripts/run-qemu.sh

Insert the Keystone driver

::

	# [inside QEMU]
	insmod keystone-driver.ko

Deploy the enclave

::

	# [inside QEMU]
	./hello/hello.ke

You'll see the enclave running!

::

	Verifying archive integrity... All good.
	Uncompressing Keystone vault archive  100%
	hello, world!

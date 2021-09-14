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
absolute path of the installed SDK.

Let's take a look at the example provided in `Keystone SDK
<https://github.com/keystone-enclave/keystone-sdk>`_.

::

	ls sdk/examples/hello

You can find two directories and ``CMakeLists.txt``.

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
	  Keystone::Enclave enclave;
	  Keystone::Params params;

	  params.setFreeMemSize(1024*1024);
	  params.setUntrustedMem(DEFAULT_UNTRUSTED_PTR, 1024*1024);

	  enclave.init(argv[1], argv[2], params);

	  enclave.registerOcallDispatch(incoming_call_dispatch);
	  edge_call_init_internals((uintptr_t) enclave.getSharedBuffer(),
	      enclave.getSharedBufferSize());

	  enclave.run();

	  return 0;
	}

``keystone.h`` contains ``Keystone::Enclave`` class which has several member functions to control the
enclave.

Following code initializes the enclave memory with the eapp/runtime.

.. code-block:: cpp

	Keystone::Enclave enclave;
	Keystone::Params params;
	enclave.init(<eapp binary>, <runtime binary>, params);

``Keystone::Params`` class is defined in ``sdk/include/host/params.h``, and contains enclave parameters
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

``CMakeLists.txt`` contains packaging commands using ``makeself``.
``makeself`` generates a self-extracting archive with a start-up command.

In order to build the example, try the following in the build directory:

::

  make hello-package

This will generate an enclave package named ``hello.ke`` under ``<build directory>/examples/hello``.
``hello.ke`` is an self-extracting archive file for the enclave.

Next, copy the package into the buildroot overlay directory.

::

  # in the build directory
  cp examples/hello/hello.ke ./overlay/root

Running ``make image`` in your build directory will generate the buildroot disk
image containing the copied package.

::

	# in your <build directory>
	make image

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
	./hello.ke

You'll see the enclave running!

::

	Verifying archive integrity... All good.
	Uncompressing Keystone Enclave Package
	hello, world!

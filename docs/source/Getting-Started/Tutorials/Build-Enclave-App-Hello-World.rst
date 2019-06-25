Tutorial 2: Hello, World!
======================================

This tutorial explains how to build and run a simple "hello world" enclave.
An enclave consists of an eapp and a runtime, but also needs the host that initializes and launches
the enclave.
Thus, each enclave source tree contains at least the host and eapp.

Before jumping into the tutorial, please complete :doc:`Quick Start
<../Running-Keystone-with-QEMU>`.


Prerequisite
------------------------------

`musl-libc <https://www.musl-libc.org/>`_ is a lightweight standard library for replacing ``glibc``.
The Eyrie runtime allows the enclave to be statically linked with musl, and supports a few standard
functions such as ``printf``.

Download source code in your work directory, and build musl with following commands.

::

	cd <your path>
	git clone https://github.com/riscv/riscv-musl
	cd riscv-musl
	./configure --prefix=<install path (absolute path)> --disable-shared --enable-static
	--host=riscv64-unknown-linux-gnu
	make; make install

``musl-gcc`` will be installed at ``<install path>``.

Open ``sdk/examples/hello/eapp/Makefile`` and set ``CC`` to point ``musl-gcc``

:: 

	CC=<install path>/bin/musl-gcc


Finally, set ``PATH`` to include RISC-V tools and ``KEYSTONE_SDK_DIR`` to point the
absolute path to ``sdk`` directory.

::

	export PATH=$PATH:<path to RISC-V tools>
	export KEYSTONE_SDK_DIR=<path to SDK>

Let's take a look at the example provided in `Keystone SDK
<https://github.com/keystone-enclave/keystone-sdk>`_.

::

	ls sdk/examples/hello

You can find two directories and a script called ``vault.sh``

vault.sh
------------------------------

``vault.sh`` is a sample script that builds the enclave.


::

	sdk/examples/hello/vault.sh

The script will compile the eapp, the runtime, and the host.
Take a look at the first few lines of the script.
In most cases, changing the variables in the script will be sufficient to build any custom enclaves.

.. code-block:: bash

	################################################################
	#                   Replace the variables                      #
	################################################################
	NAME=hello
	VAULT_DIR=`dirname $0`
	BUILD_COMMAND="make -C eapp && make -C host"
	OUTPUT_DIR=$KEYSTONE_SDK_DIR/../buildroot_overlay/root/$NAME
	EYRIE_DIR=$KEYSTONE_SDK_DIR/rts/eyrie
	EYRIE_PLUGINS="freemem untrusted_io_syscall linux_syscall env_setup"
	PACKAGE_FILES="eapp/hello \
	               host/runner \
	               $EYRIE_DIR/eyrie-rt"
	PACKAGE_SCRIPT="./runner hello eyrie-rt"

We will go through what each variable is for, and how the script builds the enclave.

``NAME`` defines the name of the enclave. It will be used to generate the final enclave package.

``VAULT_DIR`` is the path to the script. The script will change the directory to ``$VAULT_DIR``
before running the build commands.

``BUILD_COMMAND`` defines the build commands. This example has ``Makefile`` in each of the
directory, so the script just needs to execute ``make`` at each directory to build both the eapp and
the host.

``OUTPUT_DIR`` is the path to the output files. Since we started from :doc:`Quick Start
<../Running-Keystone-with-QEMU>`, we put the build outputs to the buildroot overlay directory so
that we can see them in the QEMU disk image.

``EYRIE_DIR`` is the path to the Eyrie runtime source code.

``EYRIE_PLUGINS`` defines what plugins you want to include in the runtime.

``PACKAGE_FILES`` defines which files you want to include in the final enclave package. 
``vault.sh`` uses `Makeself <https://makeself.io/>`_ to generate a self-extracting archive for the
enclave.
The package usually includes the host binary (i.e., ``host/runner``), the eapp binary (i.e.,
``eapp/hello``), and the runtime binary (i.e., ``$EYRIE_DIR/eyrie-rt``)

``PACKAGE_SCRIPT`` defines the input command to ``makeself``. The self-extracting archive will
execute this command after the decompression.

Eyrie Runtime
------------------------------

If you have completed :doc:`Quick Start <../Running-Keystone-with-QEMU>`, the runtime source code
would have been already located at ``$EYRIE_DIR``.

You can find the following command in ``vault.sh``, which builds the Eyrie runtime.

::

	$EYRIE_DIR/build.sh $EYRIE_PLUGINS


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

Vault.sh
========

``vault.sh`` is a sample build script for building eapps/hosts,
building an appropriate Eyrie configuration, and packaging all three
into a self-extracting archive.

In most cases, changing the variables in the script will be sufficient
to build a custom enclave.

The hello world
:doc:`tutorial<../Getting-Started/Tutorials/Build-Enclave-App-Hello-World>`
vault script is as follows:

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
<../Getting-Started/Running-Keystone-with-QEMU>`, we put the build outputs to the buildroot overlay directory so
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

If you have completed :doc:`Quick Start <../Getting-Started/Running-Keystone-with-QEMU>`, the runtime source code
would have been already located at ``$EYRIE_DIR``.

You can find the following command in ``vault.sh``, which builds the Eyrie runtime.

::

	$EYRIE_DIR/build.sh $EYRIE_PLUGINS




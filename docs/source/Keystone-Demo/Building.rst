Building the Demo
=================

Building the Keystone Demo application and testing it can be slightly
complex on real hardware. Testing locally with qemu is straightforward.


Building dependencies
---------------------

The demo relies on the full Keystone SDK, as well as ``libsodium`` for
cryptographic support. Currently we require two separate builds of
``libsodium``, one for the client, and one for the eapp. See the
up-to-date build instructions in the subdirectory in the demo
repository. We suggest keeping two copies of the ``libsodium``
repository to make modifications easier.


Building for qemu
-----------------

With a successfull ``libsodium`` and SDK build, building for qemu is
simple.

Set the required environement variables:

 - ``KEYSTONE_SDK_DIR``: should point to the base of the SDK repo
 - ``LIBSODIUM_DIR`` : Should point to ``libsodium/src/libsodium/`` for the eapp targeted verion
 - ``LIBSODIUM_CLIENT_DIR`` : Should  point to ``libsodium/src/libsodium/`` for the client version

You can build the regular version and remote trusted client with
``make && make trusted_client.riscv``


Running on qemu
---------------

You'll first need to copy the relevant files into the qemu image. The
easiest way to do this is to use the sdk's copy-tests functionality.

``cp *.riscv server_eapp/server_eapp.eapp_riscv ../sdk/bin/ && cd ../sdk/ && make copy-tests && cd ../keystone-demo``

To run the demo on qemu is to run both the enclave host and the
trusted client on qemu, and communicate over loopback.

Our standard testing after starting qemu is:

::
   insmod keystone-driver.ko         # load the keystone kernel module (only for newest version)
   ifdown lo && ifup lo              # Setup the loopback device
   ./enclave-host.riscv &            # Background the server host
   ./trusted_client.riscv 127.0.0.1  # Start the client interactively connecting to localhost

This will mix the output of the host and the client. ``[TC]``
indicates messages from the trusted client, ``[EH]`` is the enclave
host, and ``[SE]`` is the enclave application.

If you've modified the enclave server app, and haven't regenerated the
expected hash values, you can test with ``./trusted_client.riscv
127.0.0.1 --ignore-valid`` which will ignore the validation of the
attestation report.

Building for HiFive Unleashed
-----------------------------

First, you will need to get a working custom first-stage bootloader
(FSBL) working on your board. This will require creating a new
partition on your SD card as well as setting the MSEL2 dipswitch. See
https://github.com/sifive/freedom-u540-c000-bootloader/issues/9#issuecomment-424162283
for details.

Using the keystone-hifive-unleashed repository, build all components.

Copy the new fsbl into the partition as created above, and set the
MSEL2 switch. The board should now boot with a signed security monitor
and device keys.

At this point, you can copy over the testing binaries built in the
same way as for qemu and run them on the board.

This process can be quite long and tricky, if you run into problems
please contact us.

Running on the HiFive Unleashed
-------------------------------

First, connect the HiFive board and the client machine (laptop, etc)
to a shared network.

On the HiFive board, start the enclave host as usual: ``./enclave-host.riscv``

On the client machine, start a compatible client binary
``./trusted_client.riscv $HIFIVE_IP`` with the IP of the HiFive board.

Building a client for non-RISC-V targets
----------------------------------------

Our demo setup uses a standard Linux x86_64 laptop as the trusted client.

To build ``trusted_client`` for a non-RISC-V target, you'll need to
compile new versions of the sdk libs (libs only, not the rest) for
your target architecture.

Currently this is done by creating an additonal clone of the sdk,
modifying the Makefiles to use standard gcc/g++, and building x86_64
libs.

From there, apply the same modifications to the trusted_client
Makefile, and build the x86_64 client.

This process will be significantly streamlined in the future.

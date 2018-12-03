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

You can either build the regular version and remote trusted client
with ``make && make trusted_client.riscv`` or modify the Makefile
lines that are commented out containing refernces to the dummy client
to build a single-file test.

Copy relevant binaries (enclave-host.riscv,
enclave-host-dummy-client.riscv, server-eapp.eapp_riscv) to the
sdk/bins/ dir, then run ``make copy-tests`` in the sdk
directory. Running the qemu as normal should now have the enclave-host
binaries available.


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

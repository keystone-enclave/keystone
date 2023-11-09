.. _LaunchQEMU:

Running and Testing Keystone on QEMU
--------------------------------------

Launching Keystone in QEMU
#############################

The following command will run QEMU, starting execution from the emulated silicon root of trust.
The root of trust then jumps to the SM, and the SM boots Linux!

::

  make run

Login as ``root`` with the password ``sifive``.

You can exit QEMU by ``ctrl-a``+``x`` or using ``poweroff`` command

.. note::
  Note that the launch scripts for QEMU will start ssh on a
  forwarded localhost port. The script will print what port it has
  forwarded ssh to on start.

Start the Keystone driver by running the following command in the QEMU guest:

::

  modprobe keystone-driver


Then find the included files (i.e. enclave applications) in ``/usr/share/keystone/examples``.
To launch an enclave application, just execute it:

::

  /usr/share/keystone/examples/hello.ke

Debugging Keystone in QEMU
###############################

To debug a Keystone build with GDB, run the following command to start QEMU in debug mode:

::

  KEYSTONE_DEBUG=y make run

Then, in another terminal, run the following command to connect to the GDB server:

::

  make debug-connect

This will connect to the GDB server running in QEMU. You can then use GDB as normal to debug the
Keystone build. In addition to the normal GDB commands, the following Keystone-specific commands
are available after running ``source scripts/gdb/pmp.py`` (in GDB):

* ``pmp-clear``: Clear the PMP CSRs
* ``pmp-dump``: Show the current state of the PMP CSRs

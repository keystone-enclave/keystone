How to Debug
==============

How to debug the security monitor (bbl) and the linux kernel?
----------------------------------------------------------------

We use QEMU+GDB to debug the security monitor or the kernel.
QEMU is an effective way to debug them.

First compile with debugging symbols

.. code-block:: bash

# in your <build directory>
cmake -DCMAKE_BUILD_TYPE=Debug ..


From your `build` directory where you ran `make`, use:

.. code-block:: bash

./scripts/run-qemu.sh -debug


All cores will immediately hang at the first instruction (i.e., bootrom), waiting for `gdb` to be attached.

Now, run a riscv `gdb` in another terminal.  You can feed it with the
bbl binary or the kernel image to add debug information.


For example, if you want to debug with the `bbl` symbols

.. code-block:: bash

# in your <build directory>
riscv64-unknown-linux-gnu-gdb ./sm.build/platform/generic/firmware/fw_payload.elf


If you want to debug with the kernel's debug information

.. code-block:: bash

# in your <build directory>
riscv64-unknown-linux-gnu-gdb ./linux.build/vmlinux


Then, attach to QEMU on the port printed by the starting qemu script:

.. code-block:: bash

(gdb) target remote localhost:$PORT


Now, you can start debugging the SM (bbl) or the kernel.
Try to set breakpoints and run.

Before setting breakpoints, you should run following command:

.. code-block:: bash

(gdb) set riscv use-compressed-breakpoints no


To see why we need that command, see [this issue](https://github.com/riscv/riscv-binutils-gdb/issues/106)

Logging QEMU debug messages
---------------------------

QEMU provides a great option to collect the logs.
If you add `-D [filename]` flag to the QEMU command, it will print out the logs into `[filename]`.

You can also choose which kind of logs you want to print out, using `-d [options]` flag.
For example,

.. code-block:: bash

./riscv-qemu/riscv64-softmmu/qemu-system-riscv64 -d in_asm -D debug.log #...etc...


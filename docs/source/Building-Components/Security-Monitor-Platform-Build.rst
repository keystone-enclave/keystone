Building the Keystone Security Monitor
===========================================

The security monitor (SM) is the core component of Keystone.

The SM is implemented on top of Berkeley Bootloader (bbl).
You can find the latest SM at `<https://github.com/keystone-enclave/riscv-pk>`_ (upstream bbl:
`<https://github.com/riscv/riscv-pk>`_).
Most of the Keystone SM source code lives in ``sm`` directory.

::

	git clone https://github.com/keystone-enclave/riscv-pk
	cd riscv-pk

If you have followed :doc:`Quick Start <../Getting-Started/Running-Keystone-with-QEMU>`, the build directory is
already created at ``hifive-work/riscv-pk``.
Otherwise, create a build directory.

::

	mkdir build
	cd build

Configure and build ``bbl``. The SM can be built by additional flags ``--enable-sm`` and
``--with-target-platform`` at configuration.

::

	../configure \
		--enable-sm \
		--with-target-platform=default \
		--host=riscv64-unknown-linux-gnu \
		--with-payload=<path to vmlinux> 

	make

The Keystone Security Monitor also has an experimental port to the `Rust programming language <https://rust-lang.org>`_. If you would like to try this version of the SM, specify ``--enale-sm_rs`` instead of using ``--enable-sm``.

You may also wish to compile the SM with optimizations enabled. To do this, use the configure flag ``--enable-opt=2``.

Running ``make`` witll generate the boot image ``bbl`` in the build directory. Use it to boot the machine.

Now, we will customize the SM for a FU540 chip on HiFive Unleashed board.
Currently, we only have one experimental platform configuration for FU540.
If you wish to add your platform as a target, please submit a proposal to `GitHub <https://github.com/keystone-enclave/riscv-pk/issues>`_.

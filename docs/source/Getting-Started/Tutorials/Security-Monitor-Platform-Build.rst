Tutorial 1: Build Keystone Security Monitor
===========================================

The security monitor (SM) is the core component of Keystone.
This tutorial briefly explains how you can build the SM for your platform.

The SM is implemented on top of Berkeley Bootloader (bbl).
You can find the latest SM at `<https://github.com/keystone-enclave/riscv-pk>`_ (upstream bbl:
`<https://github.com/riscv/riscv-pk>`_).
Most of Keystone SM source code lives in ``sm`` directory.

::

	git clone https://github.com/keystone-enclave/riscv-pk
	cd riscv-pk

If you have followed :doc:`Quick Start <../Running-Keystone-with-QEMU>`, the build directory is
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

This will generate the boot image ``bbl`` in the build directory. Use it to boot the machine.

Now, we will customize the SM for a FU540 chip on HiFive Unleashed board.
The L2 controller of FU540 provides *waymasking*, which allows the SM to mask specific ways of the
L2 cache.
We have implemented an experimental cache partitioning of enclaves using this feature in order to
prevent cache side-channel attacks.

You can re-configure and re-build the ``bbl`` in the build directory:

::

	../configure \
		--enable-sm \
		--with-target-platform=fu540 \
		--host=riscv64-unknown-linux-gnu \
		--with-payload=<path to vmlinux> 

	make


Let's see how this works. In the top-level ``riscv-pk`` source directory:

::

	cd sm/platform

You will see ``default`` and ``fu540`` directories.

When you configure the ``bbl``, ``--with-target-platform`` flag specifies which files you want to
include in the SM.
For example, ``--with-target-platform=fu540`` will include ``sm/platform/fu540/fu540.c`` in the SM.
Each of the directory contains the hooks for each platform.

Let's take a look at ``fu540/fu540_internal.c``:

.. code-block:: c

	void platform_init(struct platform_enclave_data_t* ped){
	  ped->num_ways = WM_NUM_WAYS/2;
	  ped->saved_mask = 0;
	}

``platform_init`` hook is called at SM initialization.
This sets the number of ways each enclave gets.

Currently, we have hard-coded the number of ways to 8, which is the half of the L2.

On each context switch between the enclave and the host, ``platform_switch_to_enclave`` and
``platform_switch_from_enclave`` will be called.
These functions configures the way mask such that the half of the L2 is allocated to the enclave.

.. code-block:: c

	void platform_switch_to_enclave(struct platform_enclave_data_t* ped){

	  // Each hart gets special access to some
	  unsigned int core = read_csr(mhartid) + 1;

	  /* Allocate ways, fresh every time we enter */
	  size_t remaining = waymask_allocate_ways(ped->num_ways,
	                                           core,
	                                           &ped->saved_mask);

	  /* Assign the ways to all cores */
	  waymask_apply_allocated_mask(ped->saved_mask, core);

	  /* Clear out these ways MUST first apply mask to other masters */
	  waymask_clear_ways(ped->saved_mask, core);

	}
	void platform_switch_from_enclave(struct platform_enclave_data_t* ped){
	  /* Free all our ways */
	  waymask_free_ways(ped->saved_mask);

	  /* We don't need to clean them, see docs */

	}

Currently, we only have one experimental platform configuration for FU540.
If you wish to add your platform as a target, please submit a proposal to `GitHub <https://github.com/keystone-enclave/riscv-pk/issues>`_.

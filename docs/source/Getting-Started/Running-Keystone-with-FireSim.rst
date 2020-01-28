Testing Keystone with FireSim
======================================

`FireSim <https://fires.im>`_ is an FPGA-based cycle-accurate simulator for RISC-V processors.
Using FireSim, you can test Keystone on open-source processors like `RocketChip <https://github.com/freechipsproject/rocket-chip>`_
or `BOOM <https://github.com/riscv-boom/riscv-boom>`_.

Who needs it?
-----------------------

If you want to run your enclave application with Keystone, 
but you don't own any RISC-V processor, FireSim is the way to go.
FireSim allows you to simulate the processors with reasonably high speed.
You can actually boot Linux on the simulated processor and run real workloads.
You can test functionality or measure the performance of Keystone enclaves.
If you want to improve your enclave system by modifying hardware,
you can freely modify the processor hardware, and deploy it to Amazon AWS FPGAs using FireSim.

Setting Up FireSim Manager Instance
-------------------------------------

Before we start, you have to create a FireSim manager instance.
See `FireSim Documentation <https://docs.fires.im/>`_ to setup a manager instance.
Be sure to use 1.4.0 or later version of FireSim.

Building Keystone 
----------------------------------------

You can generate necessary files for FireSim with CMake.

First, clone the Keystone repository in the **manager instance**.

::
  
  git clone https://github.com/keystone-enclave/keystone

Follow :doc:`QEMU-Setup-Repository` to setup the repository.

.. note::
  FireSim's default manager instance is CentOS 7, where the default CMake package is version 2.
  In order to use CMake version 3, you need to install it by running ``sudo yum install cmake3``.
  Use ``cmake3`` instead of ``cmake``.

After you setup the repository, you can run the following commands to build Keystone.

::
  
  mkdir <build directory>
  cd <build directory>
  cmake .. -Dfiresim=y
  make
  make image

CMake with the flag ``-Dfiresim=y`` will automatically generate Makefiles to build
FireSim-compatible Linux and SM.
This includes some patches for DTB compatibility.
Also, the build will forcibly use initramfs for a simpler deployment.

Once you have built the image, you will see ``riscv-pk.build/bbl`` and
``buildroot.build/images/rootfs.ext2`` under your
build directory.

You can also boot QEMU machine with the image using ``./scripts/run-qemu.sh``.

Changing FireSim Workload Config
----------------------------------------

FireSim allows users to change their workload very easily through config files in the manager
instance.

In the FireSim's root directory, you'll see ``deploy/config_runtime.ini`` which looks as follows:

::

  # firesim/deploy/config_runtime.ini
  # ... some other configs ...
  
  [workload]
  workloadname=linux-uniform.json
  terminateoncompletion=no

``linux-uniform.json`` is a file under ``deploy/workloads/``, and defines the boot binary, rootfs and
so on:

::
  
  # firesim/deploy/workloads/linux-uniform.json
  {
    "benchmark_name" : "linux-uniform",
    "common_bootbinary" : "br-base-bin",
    "common_rootfs"     : "br-base.img",
    "common_outputs"    : ["/etc/os-release"],
    "common_simulation_outputs" : ["uartlog", "memory_stats.csv"]
  }


Now, change ``linux-uniform.json`` to ``keystone.json`` :

::
  
  # firesim/deploy/config_runtime.ini
  # ... some other configs ...
  
  [workload]
  workloadname=keystone.json
  terminateoncompletion=no


Simply, create ``keystone.json``, and make ``common_bootbinary`` point to the ``bbl`` file generated
by ``make image``. Replace ``<path/to/keystone/build/directory>`` with the path that you built
Keystone in the
previous part.

::
  
  # firesim/deploy/workloads/keystone.json
  {
    "benchmark_name" : "my-keystone"
    "common_bootbinary" : "<path/to/keystone/build/directory>/riscv-pk.build/bbl",
    "common_rootfs" : "<path/to/keystone/build/directory>/buildroot.build/images/rootfs.ext2"
    "common_simulation_outputs" : ["uartlog", "memory_stats.csv"]
  }

Although ``common_rootfs`` is not required (``bbl`` already contains the rootfs in
Linux with initramfs), FireSim does not allow it to be an empty field, thus we put the disk image we
already built.

Launching Simulation
------------------------------

Use FireSim commands to launch the simulation.
Go to the top-level FireSim directory and run:

::

  cd <path/to/firesim>
  source sourceme-f1-manager.sh

Launch runfarm and test!

::

  firesim launchrunfarm
  firesim infrasetup
  firesim boot

You can login to the f1 instance via ``ssh`` and attach to the simulated node using ``screen`` command.

See `FireSim Single Node Simulation <https://docs.fires.im/en/latest/Running-Simulations-Tutorial/Running-a-Single-Node-Simulation.html>`_ 
for more details.

::

  # [On your manager instance]
  ssh <f1 instance ip address>

::
  
  # [On the f1 instance]
  screen -r fsim0
  # [Login via root/sifive]

Running Keystone Enclaves
-----------------------------

::
  
  # [On the simulated node]
  insmod keystone-driver.ko

Run ``./tests.ke`` to run all enclaves sequentially.

::
  
  # [On the simulated node]
  ./tests.ke


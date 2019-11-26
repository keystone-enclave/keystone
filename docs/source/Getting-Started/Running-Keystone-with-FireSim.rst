Testing Keystone with FireSim
======================================

.. warning::

  This document is sligtly outdated. We'll update the document soon `GitHub Issue <https://github.com/keystone-enclave/keystone/issues/104>`_.

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
Be sure to use 1.7.0 or later version of FireSim.

Building Keystone Software
----------------------------------------

We have already packed every software required for running sample Keystone enclaves.
Add a remote to the firesim-software by executing following commands:

::
  
  cd firesim/sw/firesim-software 
  git remote add keystone https://github.com/keystone-enclave/firesim-software
  git fetch keystone

Checkout ``firesim-1.4.0-keystone`` branch and update submodules.

::
  
  git checkout firesim-1.4.0-keystone
  git submodule sync --recursive
  git submodule update --init --recursive


Build Keystone SDK
##########################

We provide sample enclaves with Keystone SDK, so let's build the enclaves and copy them to the buildroot.
Build the Keystone SDK by running following commands:

::

  cd sdk
  make all
  export KEYSTONE_SDK_DIR=$(pwd)
  

Next, we will copy the binaries into the disk image from the previous part. First, copy 
tests from the top-level keystone directory:

::

  rm -rf test
  cp -r KEYSTONE_DIR/tests .

We must modify the build file to copy the test binaries into the correct overlay directory.


::

  sed -i ’s+OUTPUT_DIR=$VAULT_DIR/../../buildroot_overlay/root/$NAME+OUTPUT_DIR=../../../workloads/br-base/overlay/root+g’ tests/tests/vault.sh

Finally, build the tests:

::

  ./tests/tests/vault.sh


Build Linux Keystone Driver
##########################
We must build and insert the keystone driver into the overlay directory. From the base directory:

::

  cd linux-keystone-driver
  sed -i ‘s+OVERLAY_DIR=../buildroot_overlay+OVERLAY_DIR=../workloads/br-base/overlay/root+g’ Makefile
  make 
  make copy

Apply Patches
##########################
We must apply patches to the Berkeley Boot Loader (BBL) and to Linux in order to ensure compatibility
with Firesim:

::

  ./keystone-setup.sh

Build Boot Image
##########################
Finally, we will use Firemarshal to build the Linux kernel with built-in Keystone module, 
and the Berkeley Bootloader (bbl) containing the Keystone security monitor.

::

  cd <path/to/firesim>
  source sourceme-f1-manager.sh
  cd sw/firesim-software
  ./marshal clean workloads/br-base.json
  ./marshal build workloads/br-base.json

Now, you should be ready to launch Firesim.

Launching Simulation
------------------------------

Use FireSim commands to launch the simulation.
Go to the top-level FireSim directory and run:

::

  cd <path/to/firesim>
  source sourceme-f1-manager.sh

Choose hardware configuration in ``deploy/config_runtime.ini``.
See `FireSim Single Node Simulation <https://docs.fires.im/en/latest/Running-Simulations-Tutorial/Running-a-Single-Node-Simulation.html>`_ 
for more details.

Currently, Keystone works on a singlecore Rocket 
(e.g., ``firesim-singlecore-no-nic-lbp``).
Use this ``runtime_config.ini`` file:

::

  # RUNTIME configuration for the FireSim Simulation Manager
  # See docs/Advanced-Usage/Manager/Manager-Configuration-Files.rst for documentation of all of these params.

  [runfarm]
  runfarmtag=mainrunfarm

  f1_16xlarges=0
  m4_16xlarges=0
  f1_2xlarges=1

  runinstancemarket=ondemand
  spotinterruptionbehavior=terminate
  spotmaxprice=ondemand

  [targetconfig]
  topology=no_net_config
  no_net_num_nodes=1
  linklatency=6405
  switchinglatency=10
  netbandwidth=200
  profileinterval=-1

  # This references a section from config_hwconfigs.ini
  # In homogeneous configurations, use this to set the hardware config deployed
  # for all simulators
  defaulthwconfig=firesim-singlecore-no-nic-lbp

  [tracing]
  enable=no
  startcycle=0
  endcycle=-1

  [workload]
  workloadname=linux-uniform.json
  terminateoncompletion=no

Launch runfarm and test!

::

  firesim launchrunfarm
  firesim infrasetup
  firesim boot

You can login to the f1 instance via ``ssh`` and attach to the simulated node using ``screen`` command.
See `FireSim Single Node Simulation <https://docs.fires.im/en/latest/Running-Simulations-Tutorial/Running-a-Single-Node-Simulation.html>`_ 
for more details.

::

  [On your manager instance]
  ssh <f1 instance ip address>
  
::

  [On the f1 instance]
  screen -r fsim0
  [Login via root/firesim]

Running Keystone Enclaves
-----------------------------

The home directory must include SDK sample enclaves and the runtime.

::
  
  [On the simulated node]
  # ls
  aes.riscv               fibonacci.eapp_riscv  test
  attestation.eapp_riscv  long-nop.eapp_riscv   test-runner.riscv
  c.eapp_riscv            loop.eapp_riscv       untrusted.eapp_riscv
  eyrie-rt                malloc.eapp_riscv
  fib-bench.eapp_riscv    stack.eapp_riscv

Run ``./test-runner.riscv`` for testing each enclave.

::

  ./test-runner.riscv stack.eapp_riscv eyrie-rt

Run ``./test`` to run all enclaves sequentially.

::
  
  ./test


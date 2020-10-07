Testing Keystone with FireSim (w/ Chipyard Framework)
=====================================================

`FireSim <https://fires.im>`_ is an FPGA-based cycle-accurate simulator for
RISC-V processors.  Using FireSim, you can test Keystone on open-source
processors like `RocketChip <https://github.com/freechipsproject/rocket-chip>`_
or `BOOM <https://github.com/riscv-boom/riscv-boom>`_.

FireSim has been integrated into  `Chipyard
<https://chipyard.readthedocs.io/en/latest/>`_, which is a framework for
hardware desigining.  We recommend you to follow the `tutorial
<https://chipyard.readthedocs.io/en/latest/Simulation/FPGA-Accelerated-Simulation.html>`_
to familiarize yourself with Chipyard and FireSim.

Who needs it?
-----------------------

If you want to run your enclave application with Keystone, but you don't own any
RISC-V processor, FireSim is the way to go.  FireSim allows you to simulate the
processors with reasonably high speed.  You can actually boot Linux on the
simulated processor and run real workloads.  You can test functionality or
measure the performance of Keystone enclaves.  If you want to improve your
enclave system by modifying hardware, you can freely modify the processor
hardware, and deploy it to Amazon AWS FPGAs using FireSim.

Setting Up Chipyard
-------------------------------------

Follow `this documentation
<https://chipyard.readthedocs.io/en/latest/Chipyard-Basics/Initial-Repo-Setup.html>`_
to setup the Chipyard repo.  Then, follow `this
<https://chipyard.readthedocs.io/en/latest/Simulation/FPGA-Accelerated-Simulation.html#firesim-sim-intro>`_
to setup FireSim.

We are going to use the latest version of `FireMarshal
<https://chipyard.readthedocs.io/en/latest/Software/FireMarshal.html>`_.  Thus,
go to the FireMarshal directory and checkout the dev branch.  The latest
FireMarshal version we tested is 1.10.0.

::

  cd <chipyard repo>
  cd software/firemarshal
  git checkout dev

.. note::

  RISC-V toolchain must be in your ``PATH`` all time. Don't forget to update
  your environment variables by ``source env.sh``.

Download Keystone Workload
----------------------------------------

We already wrote `the scripts and workload configuration for Keystone
<https://github.com/keystone-enclave/firemarshal-keystone>`_ that works with the
latest FireMarshal.  Clone the workload repo into any directory you want.

::

  git clone https://github.com/keystone-enclave/firemarshal-keystone <workload directory>

The repo includes ``keystone.json`` which you should be using for building
Keystone software stack using FireMarshal.


Build Keystone Software Stack using FireMarshal
------------------------------------------------

Now, we're ready to build the software stack of Keystone.
Go to FireMarshal directory, and build the workload.

::

  cd <chipyard repo>
  cd software/firemarshal
  ./marshal -v build <path/to/keystone.json>

You should replace ``<path/to/keystone.json>`` with the absolute path to the
``keystone.json`` in the workload directory.

You can launch QEMU to run the tests.

::

  ./marshal -v launch <path/to/keystone.json>

Finally, install the workload in FireSim.

::

  ./marshal -v install <path/to/keystone.json>

``keystone`` workload should be installed in ``<chipyard
repo>/sims/firesim/deploy/workloads``.

Launching Simulation (FireSim)
------------------------------

Open ``config_runtime.ini`` in ``<chipyard repo>/sims/firesim/deploy`` and edit
``[workload]`` section as follows

::

  [workload]
  workloadname=keystone.json
  # ...

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

You can login to the f1 instance via ``ssh`` and attach to the simulated node
using ``screen`` command.

See `FireSim Single Node Simulation
<https://docs.fires.im/en/latest/Running-Simulations-Tutorial/Running-a-Single-Node-Simulation.html>`_
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

.. warning::

  We don't support the emulated secure boot in FireSim, so the attestation key
  of security monitor will be all zeroes.  Thus, the attestation will not work
  unless you change your bootrom. See `this issue
  <https://github.com/keystone-enclave/keystone/issues/210>`_.


Run ``./tests.ke`` to run all enclaves sequentially.

::
  
  # [On the simulated node]
  ./tests.ke


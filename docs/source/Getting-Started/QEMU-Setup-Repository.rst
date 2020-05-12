Setup
----------------------------

In this stage, you will (1) install RISC-V toolchain, (2) checkout git submodules, and (3) Install
SDK.
You can either quickly setup everything (:ref:`QEMUSetupQuick`) or manually setup
(:ref:`QEMUSetupManual`).

.. _QEMUSetupQuick:

Quick Setup
########################

You can quickly setup everything by running ``./fast-setup.sh``. This script requires Git >= 2.11.0.

::

  ./fast-setup.sh

This will download pre-compiled RISC-V tools and extract it to
``riscv`` directory and setup submodules.

If you want to compile RISC-V tools from source code, run
``./setup.sh`` instead. This may be necessary on some platforms due to
library issues.

To keep environment variables, add following lines to your ``.bashrc``.

::

  export RISCV=<path/to/keystone>/riscv
  export PATH=$PATH:$RISCV/bin
  export KEYSTONE_SDK_DIR=<path/to/keystone>/sdk

You can also manually run ``source source.sh`` to set the environment variables.

.. _QEMUSetupManual:

Manual Setup
########################

If you used ``./fast-setup.sh`` or ``./setup.sh``, you may skip this section.
This section explains the manual way of setting up the repository.

First, sync and update all the submodules.
You may need these commands whenever you checkout to a different branch (e.g., ``dev``, ``master``,
etc).

::

  git submodule sync --recursive
  git submodule update --init --recursive

For additional inormation, see `git submodules <https://git-scm.com/book/en/v2/Git-Tools-Submodules>`_.

Install the RISC-V GNU toolchain for cross compilation.
See `riscv-gnu-toolchain <https://github.com/riscv/riscv-gnu-toolchain>`_ for details.

::

  cd riscv-gnu-toolchain
  ./configure --prefix=$(pwd)/../riscv
  make; make linux

Install the Keystone SDK and prepare Eyrie runtime source code.

::

   cd sdk
   mkdir build
   cd build
   cmake .. -DOUTPUT_DIR=$(pwd)/../lib
   make
   make install

Initialize the runtime (i.e., Eyrie) source code.

::

  ./sdk/scripts/init.sh

This initializes the runtime source code at ``./sdk/rts`` based on the version specified in
``./sdk/rts/eyrie.version``.

For more usage of the script, try the script with ``--help`` flag.

To keep environment variables, add following lines to your ``.bashrc``.

::

  export RISCV=<path/to/keystone>/riscv
  export PATH=$PATH:$RISCV/bin
  export KEYSTONE_SDK_DIR=<path/to/keystone>/sdk

You can also manually run ``source source.sh`` to set the environment variables.

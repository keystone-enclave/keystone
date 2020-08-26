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

The script also installs Keystone SDK if ``KEYSTONE_SDK_DIR`` environment variable is not set.
The default install directory of Keystone SDK is ``$(pwd)/sdk/build``.
If you want to change the install directory,
please follow `SDK's README <https://github.com/keystone-enclave/keystone-sdk/blob/master/README.md>`_
before running ``fast-setup.sh``.

If everything went well, you should see the following message:

::

  RISC-V toolchain and Keystone SDK have been fully setup

After you run ``fast-setup.sh``, run the following command to set relevant environment variables:

::

  source source.sh

To keep the environment variables, add the lines in ``source.sh`` to your shell's startup file.
For example, if you're using bash, then try:

::

  cat source.sh >> $HOME/.bashrc

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

Install the Keystone SDK by following
`SDK's README <https://github.com/keystone-enclave/keystone-sdk/blob/master/README.md>`_.

::

   cd build
   export KEYSTONE_SDK_DIR=<keystone/sdk/install/directory>
   cmake ..
   make
   make install

To keep environment variables, add following lines to your ``.bashrc``.

::

  export RISCV=<path/to/keystone>/riscv
  export PATH=$PATH:$RISCV/bin
  export KEYSTONE_SDK_DIR=<keystone/sdk/install/directory>

You can also manually run ``source source.sh`` to set the environment variables.

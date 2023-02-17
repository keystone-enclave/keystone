Setup
----------------------------

In this stage, you will (1) install RISC-V toolchain, (2) checkout git submodules, and (3) Install
SDK.
You can either quickly setup everything (:ref:`QEMUSetupQuick`) or manually setup.

.. _QEMUSetupQuick:

Quick Setup
########################

You can quickly setup everything by running ``./fast-setup.sh``. This script requires Git >= 2.11.0.

If you want to use a different version of toolchain (e.g., compiled the toolchain from source),
make sure that your toolchain binaries are in your PATH before running this command.

::

  ./fast-setup.sh

This will download pre-compiled RISC-V tools and extract it to
``riscv64`` directory and setup submodules.

.. note::
  If you want RV32 setup, you should do ``BITS=32 ./fast-setup.sh`` instead. The toolchain will be
  installed in ``riscv32`` directory. Also, the SDK will be compiled and installed using RV32 toolchain.

The script also installs Keystone SDK if ``KEYSTONE_SDK_DIR`` environment variable is not set.
The default install directory of Keystone SDK is ``$(pwd)/sdk/build64`` (``$(pwd)/sdk/build32`` for RV32).
If you want to change the install directory,
please follow `SDK's README <https://github.com/keystone-enclave/keystone/blob/master/sdk/README.md>`_
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


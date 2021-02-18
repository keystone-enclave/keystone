Testing Keystone with QEMU
====================================

`QEMU <https://www.qemu.org>`_ is an open source machine emulator.
The latest QEMU supports RISC-V ISA.

Keystone was tested in a QEMU fork in RISC-V (`GitHub <https://github.com/riscv/riscv-qemu>`_).

If you just want to quickly test Keystone with QEMU, you can start with Docker, which will take <10
minutes.
Setting the environment by yourself (without Docker) will take 1-2 hours.

Start With Docker
-----------------------

The fastest way to try Keystone is through our `Docker <https://www.docker.com/>`_ image (Ubuntu 18.04).
The image contains all of our source code, as well as a compiled kernel, QEMU, example binaries, SDK, and GNU tools.

You may need to install Docker in your machine before you start.

We publish Docker images via `Docker Hub repositories <https://hub.docker.com/orgs/keystoneenclaveorg/repositories>`_.
The following command will pull an image with everything. Replace ``<tag>`` with the image tag you want to pull.
For example, if you want to pull an image of ``master`` branch,

::

  docker pull keystoneenclaveorg/keystone:master

The image with ``dev`` tag contains the dev branch.

To run all the tests, you can simply do

::

  docker run keystoneenclaveorg/keystone:master

Or, to interact with the container via bash, do

::

  docker run -it --entrypoint /bin/bash keystoneenclaveorg/keystone:master

In the interactive mode, you can run tests with the following commands:

::

  # In the container
  cd /keystone
  source source.sh
  cd build
  make run-tests

Start Without Docker
-----------------------

.. toctree::
   :maxdepth: 1

   ./Install-Dependencies
   ./QEMU-Setup-Repository
   ./QEMU-Compile-Sources
   ./QEMU-Run-Tests

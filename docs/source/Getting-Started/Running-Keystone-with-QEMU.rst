Testing Keystone with QEMU
====================================

`QEMU <https://www.qemu.org>`_ is an open source machine emulator.
The latest QEMU supports RISC-V ISA.

Keystone was tested in a QEMU fork in RISC-V (`GitHub <https://github.com/riscv/riscv-qemu>`_).
The upstream QEMU is not currently supported due to several remaining issues (Ex: `See GitHub issue <https://github.com/keystone-enclave/keystone/issues/25>`_).
Fixes will be upstreamed in the future.

If you just want to quickly test Keystone with QEMU, you can start with Docker, which will take <10
minutes.
Setting the environment by yourself (without Docker) will take 1-2 hours. 

Start With Docker
-----------------------
The fastest way to try Keystone is through our `Docker <https://www.docker.com/>`_ image (Ubuntu 16.04/18.04).
The image contains all of our source code, 
as well as a compiled kernel, QEMU, example binaries, SDK, and GNU tools.
Note that the source code in the Docker image may contain the latest release, not the latest branch.
Always make sure you are working with the right version that matches with the documentation.

Prerequisite: Docker

Download the image and run a container.
::

  wget https://keystone-enclave.eecs.berkeley.edu/files/keystone-sample-image_docker.tgz
  docker load --input keystone-sample-image_docker.tgz
  docker run --name keystone-sample -it keystone-sample-image

Now, run tests in the container. See :doc:`./QEMU-Run-Tests` for details.
::
  
  # In the container
  su keystone
  cd ~/keystone
  make run-tests 

Start Without Docker
-----------------------

.. toctree::
   :maxdepth: 1

   ./Install-Dependencies
   ./QEMU-Setup-Repository
   ./QEMU-Compile-Sources
   ./QEMU-Run-Tests

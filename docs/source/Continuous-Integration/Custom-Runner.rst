Custom CI Runner
===========

We have created a custom CI runner set up with real RISC-V hardware so that we
can run CI tests even on platforms that don't have a good QEMU-based simulator.
This document details how this runner was setup so that it can be replicated or
extended with future hardware platforms.

Setup
-----------------

On the machine which has the RISC-V board connected, first create a new user and
assign them to any groups they will need in order to talk to the hardware.

.. code-block:: bash

    sudo adduser runner
    sudo usermod -aG disks runner
    sudo usermod -aG dialout runner
    ...
    # To allow user systemd units to run even when user is not logged in
    sudo loginctl enable-linger runner

Then, login as that user and follow `these <https://docs.github.com/en/actions/hosting-your-own-runners/managing-self-hosted-runners/adding-self-hosted-runners>`_
instructions for creating a new custom runner. When setting up the custom runner,
you will want to add any tags as necessary to describe the hardware that this
runner has access to. Then, optionally (but recommended), create a new systemd
unit file to automatically start the runner on machine boot.

.. code-block:: bash

    mkdir -p /home/runner/.config/systemd/user
    cat <<EOF > /home/runner/.config/systemd/user/runner.service
    [Service]
    ExecStart=/home/runner/actions-runner/run.sh

    [Install]
    WantedBy=default.target
    EOF

    systemctl --user enable runner
    systemctl --user start runner

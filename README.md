# Keystone Enclave: An Open-Source Secure Enclave for RISC-V Processors

![Documentation Status](https://readthedocs.org/projects/keystone-enclave/badge/)
[![Build Status](https://travis-ci.org/keystone-enclave/keystone.svg?branch=master)](https://travis-ci.org/keystone-enclave/keystone/)

Visit [Project Website](https://keystone-enclave.org) for more information.

The ``dev`` branch is more up-to-date than ``master`` always. Until
there are stable releases we suggest only using ``dev``.

# Quick Start

```bash
git clone https://github.com/keystone-enclave/keystone
cd keystone
```

Install Dependencies (Ubuntu)

```
sudo apt update
sudo apt install autoconf automake autotools-dev bc bison build-essential curl \
expat libexpat1-dev flex gawk gcc git gperf libgmp-dev libmpc-dev libmpfr-dev \
libtool texinfo tmux patchutils zlib1g-dev wget bzip2 patch vim-common lbzip2 \
python pkg-config libglib2.0-dev libpixman-1-dev device-tree-compiler expect
```

Checkout branch (optional)
```
git checkout dev
```

Setup Tools and Submodules
```
./fast-setup.sh
```

Build All
```
make
```

Run QEMU
```
./scripts/run-qemu.sh
```

Test

login with `root`/`sifive`.

```
[in QEMU machine]
insmod keystone-driver.ko
./test
```

Terminate QEMU
```
poweroff
```

## Rebuilding

Any modifications in a submodule should be built with the top-level
`make` in Keystone.

# Documentation

See [docs](http://docs.keystone-enclave.org) for detailed
documentation. You may wish to switch to the dev version of the docs,
rather than 'latest'.

# Contributing

See CONTRIBUTING.md

# Keystone: An Open-Source Secure Enclave Framework for RISC-V Processors

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
python pkg-config libglib2.0-dev libpixman-1-dev device-tree-compiler expect \
libssl
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

##Test

Build SDK Tests
```
make sdk-tests
```

Rebuild Image
```
make
```

Run QEMU
```
./scripts/run-qemu.sh
```

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

Modifications to SDK tests require running
```
make sdk-tests
```
or running the build-and-copy scripts for the individual tests.

## Adding Files

Any additional files/binaries to be included in the image should be
placed in `keystone/buildroot_overlay/root/`.

The top-level `make` will rebuild the image to contain any new files
added.

# Documentation

See [docs](http://docs.keystone-enclave.org) for detailed
documentation. You may wish to switch to the dev version of the docs,
rather than 'latest'.

# Contributing

See CONTRIBUTING.md

# Keystone Enclave: An Open-Source Secure Enclave for RISC-V Processors

![Documentation Status](https://readthedocs.org/projects/keystone-enclave/badge/)
[![Build Status](https://travis-ci.org/keystone-enclave/keystone.svg?branch=master)](https://travis-ci.org/keystone-enclave/keystone/)

Visit [Project Website](https://keystone-enclave.org) for more information.

# Quick Start

```bash
git clone https://github.com/keystone-enclave/keystone
cd keystone
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

login with `root`/`busybear`.

```
[in QEMU machine]
insmod keystone-driver.ko
./test
```

Terminate QEMU
```
poweroff
```

# Documentation

See [docs](http://docs.keystone-enclave.org) for detailed documentation.

# Contributing

See CONTRIBUTING.md

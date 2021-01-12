# Keystone Security Monitor

This repository contains Keystone Security Monitor (SM) which is
the core software of [Keystone enclave](https://keystone-encloave.org) framework.

The security monitor has been originally implemented on top of the Berkeley Boot Loader (BBL),
but we decided to port it over to OpenSBI.
To see the old BBL version, please go to https://github.com/keystone-enclave/riscv-pk

We have changed all of our SBI functions to follow OpenSBI's SBI specification.
To see the spec, please see `spec` directory.

This version does not contain the Rust implementation that was in BBL version.
We will port the Rust version in the future

## Build

You can build the bootloader firmware with Keystone security monitor using OpenSBI's out-of-tree platform build.

```
make -C opensbi O=<build dir> PLATFORM_DIR=$(pwd)/plat/generic CROSS_COMPILE=riscv64-unknown-elf- FW_PAYLOAD_PATH=<path/to/linux/image> FW_PAYLOAD=y
```

Replace <build dir> with your build directory path and <path/to/linux/image> with Linux image.

The firmware will be generated under `<build dir>/platform/generic/firmware`

## Test

WIP

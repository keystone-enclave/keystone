# Keystone Security Monitor

This repository contains Keystone Security Monitor (SM) which is
the core software of [Keystone enclave](https://keystone-encloave.org) framework.

The security monitor has been originally implemented on top of the Berkeley Boot Loader (BBL),
but we decided to port it over to OpenSBI.
To see the old BBL version, please go to https://github.com/keystone-enclave/riscv-pk

We have changed all of our SBI functions to follow OpenSBI's SBI specification.
To see the spec, please see `spec` directory.

This version does not contain the Rust implementation that was in BBL version.
We will port the Rust version in the future.

## Initialize

Update the submodules.

```
git submodule update --init
```

OpenSBI is a submodule of this repository.
We periodically check for new OpenSBI versions for bump.

## Build

You can build the bootloader firmware with Keystone security monitor using OpenSBI's out-of-tree platform build.

```
make -C opensbi O=<build dir> PLATFORM_DIR=$(pwd)/plat/generic CROSS_COMPILE=riscv64-unknown-elf- FW_PAYLOAD_PATH=<path/to/linux/image> FW_PAYLOAD=y
```

In order to build 32-bit firmware, try:

```
make -C opensbi O=<build dir> PLATFORM_DIR=$(pwd)/plat/generic CROSS_COMPILE=riscv32-unknown-elf- FW_PAYLOAD_PATH=<path/to/linux/image> FW_PAYLOAD=y PLATFORM_RISCV_XLEN=32
```

Replace <build dir> with your build directory path and <path/to/linux/image> with Linux image.

For other platforms, please refer to the [Keystone documentation](https://docs.keystone-enclave.org).

The firmware will be generated under `<build dir>/platform/generic/firmware`

## Unit Test

Our unit tests are implemented with CMocka, and will run on RISC-V QEMU.
Please make sure `qemu-riscv64` (or `qemu-riscv32` for rv32) is in your PATH.
You can also download the prebuilt QEMU by:

```
wget https://keystone-enclave.eecs.berkeley.edu/files/qemu-riscv64
```

The test requires patched OpenSBI firmware because of software-simulated secure boot keys.
The patch is under `tests/patch`.
Please apply any patches under the directory to the opensbi submodule.

To run the tests, try the following:

```
cd tests
mkdir build
cd build
cmake ..
make test
```

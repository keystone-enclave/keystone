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

## Hash Generation

In order to perform remote attestation and verify the security monitor,
you need an expected 64-byte hash of the security monitor firmware image.
We provide a simple tool for generating a header `sm_expected_hash.h` that contains
the expected hash for a given firmware image.

```
cd tools
make
make hash FW_PATH=<firmware path>
```

Where `<firmware path>` is the path containing OpenSBI's firmware images
(i.e., `fw_payload.elf` and `fw_payload.bin`)

The default `<firmware path>` is `../../build/sm.build/platform/generic/firmware`.
Thus, if you have already built the security monitor in the Keystone build directory, you can just do

```
make hash
```

You can see the generated `sm_expected_hash.h` that you can use for the remote attestation.

Here is an example:
```cpp
unsigned char sm_expected_hash[] = {
  0x63,0x6a,0xc1,0x7c,0x15,0xb4,0x68,0xb9,
  0x48,0x14,0xc7,0xaf,0xad,0xba,0xd3,0xc4,
  0x57,0xd1,0xe3,0x68,0xc1,0x83,0x10,0xbd,
  0x0d,0x9d,0x43,0x93,0x72,0xc2,0xc7,0x81,
  0x27,0x17,0xb1,0x3f,0xda,0x8e,0x12,0x33,
  0x5e,0xfe,0xdb,0xbc,0x5d,0x84,0x55,0x8f,
  0xa3,0xb9,0x80,0xb2,0x47,0x87,0x67,0x1e,
  0xcc,0x81,0x4a,0x8f,0xce,0xb3,0x30,0x1e,};
unsigned int sm_expected_hash_len = 64;
```

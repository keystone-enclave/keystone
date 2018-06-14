# Keystone Enclave
## Introduction

Keystone Enclave is an open-source secure hardware enclave based on RISC-V.
Visit [Project Website](https://keystone-enclave.org) for more information.

## Compile Sources

**Clone and init the repo**
```bash
git clone https://github.com/keystone-enclave/keystone
cd keystone
git submodule update --init --recursive
```

**Install RISC-V GNU Toolchain**
```bash
mkdir riscv
export RISCV=$(pwd)/riscv
cd riscv-gnu-toolchain
./configure --prefix=$RISCV
make linux
cd ..
```

**Busybear**
Refer to https://github.com/michaeljclark/busybear-linux for more information
```bash
cd busybear-linux
make
cd ..
```

**RISC-V QEMU**
```bash
cd riscv-qemu
./configure --target-list=riscv64-softmmu,riscv32-softmmu
make
cd ..
```
**RISC-V Linux**
```bash
cd riscv-linux
cp ../busybear-linux/conf/linux.config .config
make ARCH=riscv olddefconfig
make ARCH=riscv vmlinux
cd ..
```
**Security Monitor (bbl)**
```bash
cd riscv-pk
mkdir build
cd build
../configure \
    --enable-logo \
    --host=riscv64-unknown-elf \
    --with-payload=../../riscv-linux/vmlinux \
    --enable-sm
make
```
Make sure to add `--enable-sm` to `configure`, so that the security monitor is included in the bbl

**Bootloader (for boot ROM)**
```bash
cd sanctum_bootloader
make 
cd ..
```

## Run Simulation (QEMU)

```bash
./run-qemu.sh
```
Login as `root` with the password `busybear`
You can exit the QEMU by `ctrl-b x`


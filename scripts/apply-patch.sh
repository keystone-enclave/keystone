#!/usr/bin/env bash
cd riscv-qemu
patch --forward hw/riscv/boot.c < ../patches/qemu-kernel-reloc.patch || true
cd ..

# linux patch
cd riscv-linux
patch -p0 --forward <  ../patches/linux-cma.patch || true
cd ..


#!/usr/bin/env bash
cd riscv-qemu
patch -p0 --forward hw/riscv/boot.c < ../patches/qemu-kernel-reloc.patch || true

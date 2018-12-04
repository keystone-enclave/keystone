#!/usr/bin/env bash
cd riscv-qemu
patch hw/riscv/boot.c < ../patches/qemu-kernel-reloc.patch

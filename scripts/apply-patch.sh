cd riscv-qemu
patch hw/riscv/boot.c < ../patches/qemu-kernel-reloc.patch
patch target/riscv/pmp.c < ../patches/pmp-bug.patch

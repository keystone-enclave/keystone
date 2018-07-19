cd riscv-qemu
patch target/riscv/pmp.c < ../patches/riscv-pmp-napot.patch
patch target/riscv/pmp.c < ../patches/riscv-pmp-range.patch


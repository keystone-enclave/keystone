#!/usr/bin/env bash
./riscv-qemu/riscv64-softmmu/qemu-system-riscv64 -S -s -D debug.log -nographic -machine virt \
	-secure-boot sanctum_bootloader/boot_trng.bin \
  -kernel riscv-pk/build/bbl -append "root=/dev/vda ro console=ttyS0" \
  -drive file=busybear-linux/busybear.bin,format=raw,id=hd0 \
  -device virtio-blk-device,drive=hd0 \
  #-netdev type=tap,script=./ifup,downscript=./ifdown,id=net0 \
  -device virtio-net-device,netdev=net0 \

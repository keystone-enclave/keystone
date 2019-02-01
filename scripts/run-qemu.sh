#!/usr/bin/env bash
./riscv-qemu/riscv64-softmmu/qemu-system-riscv64 -D debug.log -nographic -m 4G -machine virt\
	-bios bootrom/bootrom.elf \
  -kernel riscv-pk/build/bbl -append "root=/dev/vda ro console=ttyS0" \
  -drive file=busybear-linux/busybear.bin,format=raw,id=hd0 \
  -device virtio-blk-device,drive=hd0 \
  -netdev  user,id=net0,net=192.168.100.1/24,dhcpstart=192.168.100.128,hostfwd=tcp::5555-:22 \
  -device  virtio-net-device,netdev=net0

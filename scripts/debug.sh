#./riscv-qemu/riscv64-softmmu/qemu-system-riscv64 -d in_asm \ #in_asm,out_asm,int,mmu,exec,cpu
#gdb --args ./riscv-qemu/riscv64-softmmu/qemu-system-riscv64 -d in_asm -D debug.log -nographic -machine virt \
./riscv-qemu/riscv64-softmmu/qemu-system-riscv64 \
  -S -s -d in_asm -D debug.log -nographic -machine virt \
	-bios sanctum_bootloader/boot_trng.elf \
  -kernel riscv-pk/build/bbl -append "root=/dev/vda ro console=ttyS0" \
  -drive file=busybear-linux/busybear.bin,format=raw,id=hd0 \
  -device virtio-blk-device,drive=hd0 \
  #-netdev type=tap,script=./ifup,downscript=./ifdown,id=net0 \
  #-device virtio-net-device,netdev=net0 \

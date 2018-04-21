gdb --args ./qemu-system-riscv64 -d in_asm,out_asm,int,mmu,exec,cpu -D debug.log -nographic -machine virt \
	-secure-boot sanctum_bootloader/boot_trng.bin \
  -kernel bbl -append "root=/dev/vda ro console=ttyS0" \
  -drive file=busybear.bin,format=raw,id=hd0 \
  -device virtio-blk-device,drive=hd0 \
  #-netdev type=tap,script=./ifup,downscript=./ifdown,id=net0 \
  -device virtio-net-device,netdev=net0 \

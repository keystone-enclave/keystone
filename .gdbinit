set auto-load safe-path /
set riscv use-compressed-breakpoints no
add-symbol-file ./build/bootrom.build/bootrom.elf
add-symbol-file ./build/sm.build/platform/generic/firmware/fw_jump.elf
dir ../keystone-runtime

#add-symbol-file ./sdk/cmake-build-debug/examples/ports/sender
#add-symbol-file ./sdk/cmake-build-debug/examples/ports/ports-runner
#add-symbol-file ./sdk/cmake-build-debug/examples/ports/receiver
#add-symbol-file ./sdk/cmake-build-debug/examples/ports/eyrie-rt
add-symbol-file ./build/linux.build/vmlinux
source ./build/linux.build/vmlinux-gdb.py
target remote :9822

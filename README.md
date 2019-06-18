# Keystone Enclave Driver

This is a loadable kernel module for Keystone Enclave.  To build the
module, make with the top-level
[Keystone](https://github.com/keystone-enclave/keystone) build
process.

# Compatibility

The driver will always work correctly with the version of riscv-linux
pointed to by the top-level
[Keystone](https://github.com/keystone-enclave/keystone) repository.

For the upstream linux, loadable modules for RISC-V only work on kernel versions later than 4.17.

To use the module in 4.15, please use this version

https://github.com/riscv/riscv-linux/tree/65e929792fb9b632c20be118fa0795b26cc89fe4

If you are using kernel earlier than 4.15, you might need to apply Zong's patch by yourself.

https://lore.kernel.org/patchwork/patch/933133/

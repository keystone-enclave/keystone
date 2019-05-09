# How to Debug

## How to debug the security monitor (bbl) and the linux kernel?

We use QEMU+GDB to debug the security monitor or the kernel.
QEMU is an effective way to debug them.

```bash
./scripts/run-qemu.sh -debug
```

All cores will immediately hang at the first instruction (i.e., bootrom), waiting for `gdb` to be attached.

Now, run a riscv `gdb` in another terminal.  You can feed it with the
bbl binary or the kernel image to add debug information.  (You may
want to compile them with the debugging flag `-g`)

For example, if you want to debug with the `bbl` symbols

```bash
riscv64-unknown-linux-gnu-gdb ./hifive-work/riscv-pk/bbl
```

If you want to debug with the kernel's debug information

```bash
riscv64-unknown-linux-gnu-gdb ./riscv-linux/vmlinux
```

Then, attach to QEMU:

```bash
(gdb) target remote localhost:1234
```

Now, you can start debugging the SM (bbl) or the kernel.
Try to set breakpoints and run.

Before setting breakpoints, you should run following command:

```
(gdb) set riscv use_compressed_breakpoints no
```

To see why we need that command, see [this issue](https://github.com/riscv/riscv-binutils-gdb/issues/106)

## Logging QEMU debug messages

QEMU provides a great option to collect the logs.
If you add `-D [filename]` flag to the QEMU command, it will print out the logs into `[filename]`.

You can also choose which kind of logs you want to print out, using `-d [options]` flag.
For example,

```bash
./riscv-qemu/riscv64-softmmu/qemu-system-riscv64 -d in_asm -D debug.log #...etc...
```

BINS_DIR = bin/
DISK_IMAGE = ../busybear-linux/busybear.bin
MOUNT_DIR = ./tmp_busybear
RUNTIME = eyrie-rt

OPTIONS_FLAGS ?= -DIO_SYSCALL_WRAPPING -DLINUX_SYSCALL_WRAPPING -DUSE_FREEMEM -DENV_SETUP
#OPTIONS_FLAGS += -DDYN_ALLOCATION
#OPTIONS_FLAGS += -DINTERNAL_STRACE
export OPTIONS_FLAGS


all:


	$(MAKE) -C lib
	$(MAKE) -C runtime
	$(MAKE) -C tests
	$(MAKE) -C samples
# Ensure bins are also in the bins dir
	mkdir -p $(BINS_DIR)

	cp tests/*/*.eapp_riscv tests/test tests/test-runner.riscv $(BINS_DIR)
	cp samples/tiny-AES-c/aes.riscv $(BINS_DIR)

	cp runtime/$(RUNTIME) $(BINS_DIR)

clean:
	rm -rf bin
	$(MAKE) -C lib clean
	$(MAKE) -C runtime clean
	$(MAKE) -C tests clean
	$(MAKE) -C samples clean

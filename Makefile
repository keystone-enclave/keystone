BINS_DIR = bin/
DISK_IMAGE = ../busybear-linux/busybear.bin
MOUNT_DIR = ./tmp_busybear
RUNTIME = eyrie-rt

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


copy-tests:
	$(MAKE) -C tests
# Copy into the mount
	mkdir -p $(MOUNT_DIR)
	sudo mount $(DISK_IMAGE) $(MOUNT_DIR)
	sudo rsync -a $(BINS_DIR)/ $(MOUNT_DIR)/root/
	sudo umount $(MOUNT_DIR)
	rmdir $(MOUNT_DIR)

clean:
	rm -rf bin
	$(MAKE) -C lib clean
	$(MAKE) -C runtime clean
	$(MAKE) -C tests clean
	$(MAKE) -C samples clean

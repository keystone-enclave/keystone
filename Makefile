BINS_DIR = bin/
DISK_IMAGE = ../busybear-linux/busybear.bin
MOUNT_DIR = ./tmp_busybear
RUNTIME = eyrie-rt

all:
	make -C host
	make -C app
	make -C runtime
	make -C tests

copy-tests:
	mkdir -p $(BINS_DIR)
	make -C tests
	# copy test binaries
	cp tests/*/*.eapp_riscv tests/test tests/test-runner.riscv $(BINS_DIR)
	# copy runtime
	cp runtime/$(RUNTIME) $(BINS_DIR)
	mkdir -p $(MOUNT_DIR)
	sudo mount $(DISK_IMAGE) $(MOUNT_DIR)
	sudo rsync -a $(BINS_DIR)/ $(MOUNT_DIR)/root/
	sudo umount $(MOUNT_DIR)
	rmdir $(MOUNT_DIR)

clean:
	rm -rf bin
	make -C host clean
	make -C app clean
	make -C runtime clean
	make -C tests clean

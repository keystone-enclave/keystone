OVERLAY_DIR=../buildroot_overlay
DRIVER = keystone-driver.ko

ifneq ($(KERNELRELEASE),)
	keystone-driver-y := \
		keystone.o \
		keystone-page.o \
		keystone-ioctl.o \
		keystone-enclave.o
	obj-m += keystone-driver.o
else

PWD := $(shell pwd)
KDIR := $(PWD)/../riscv-linux

default:
	$(MAKE) -C $(KDIR) ARCH=riscv SUBDIRS=$(PWD) modules

copy:
	cp $(DRIVER) $(OVERLAY_DIR)


endif

clean:
	rm -rvf *.o *.ko *.order *.symvers *.mod.c .tmp_versions .*o.cmd

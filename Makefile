ifneq ($(KERNELRELEASE),)
	keystone-driver-y := \
		keystone.o \
		keystone-rtld.o \
		keystone-page.o \
		keystone-ioctl.o \
		keystone-enclave.o
	obj-m += keystone-driver.o
else

PWD := $(shell pwd)
KDIR := $(PWD)/../riscv-linux/

default:
	$(MAKE) -C $(KDIR) ARCH=riscv SUBDIRS=$(PWD) modules

endif

clean:
	rm -rvf *.o *.ko *.order *.symvers *.mod.c .tmp_versions .*o.cmd

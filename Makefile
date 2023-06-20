
#############################
## Configuration variables ##
#############################

export SHELL := /bin/bash

export KEYSTONE                 ?= $(dir $(abspath $(firstword $(MAKEFILE_LIST))))
export KEYSTONE_BUILDROOT       ?= $(KEYSTONE)/buildroot
export KEYSTONE_BR2_EXT         ?= $(KEYSTONE)/overlays

export KEYSTONE_DRIVER          ?= $(KEYSTONE)/linux-keystone-driver
export KEYSTONE_EXAMPLES        ?= $(KEYSTONE)/examples
export KEYSTONE_RUNTIME         ?= $(KEYSTONE)/runtime
export KEYSTONE_SDK             ?= $(KEYSTONE)/sdk
export KEYSTONE_BOOTROM         ?= $(KEYSTONE)/bootrom
export KEYSTONE_SM              ?= $(KEYSTONE)/sm

export BUILDDIR                 ?= $(KEYSTONE)/build
export BUILDROOT_BUILDDIR       ?= $(BUILDDIR)/buildroot.build

export KEYSTONE_PLATFORM        ?= generic
export KEYSTONE_BITS            ?= 64

#ifeq ($(RISCV),)
        # todo maybe unnecessary depending on how we build toolchain
#        $(error "Set RISCV environment variable. Try cd $(KEYSTONE) ; source source.sh")
#endif

include mkutils/args.mk
include mkutils/log.mk

# Buildroot configuration variables
BUILDROOT_MAKEFLAGS     := -C $(KEYSTONE_BUILDROOT) O=$(BUILDROOT_BUILDDIR)

# Collect external flags
EXTERNALS := keystone
ifeq ($(KEYSTONE_PLATFORM),mpfs)
	EXTERNALS += microchip
endif

#$(error $(addprefix $(KEYSTONE_BR2_EXT)/,$(EXTERNALS)))

BUILDROOT_MAKEFLAGS     += BR2_EXTERNAL=$(call SEPERATE_LIST,:,$(addprefix $(KEYSTONE_BR2_EXT)/,$(EXTERNALS)))
BUILDROOT_CONFIGFILE    ?= qemu_riscv$(KEYSTONE_BITS)_virt_defconfig

#####################
## Generic targets ##
#####################

all: buildroot

$(BUILDDIR):
	mkdir -p $@

###############
## Buildroot ##
###############

# Build directory

$(BUILDROOT_BUILDDIR): $(BUILDDIR)
	mkdir -p $@

# Configuration

$(BUILDROOT_BUILDDIR)/.config: $(BUILDROOT_BUILDDIR)
	$(call log,info,Configuring Buildroot with $(BUILDROOT_CONFIGFILE))
	$(MAKE) $(BUILDROOT_MAKEFLAGS) $(BUILDROOT_CONFIGFILE)

# Main build target for buildroot. The specific subtarget to build can be overriden
# by setting the BUILDROOT_TARGET environment variable.

BUILDROOT_TARGET        ?= all

.PHONY: buildroot
buildroot: $(BUILDROOT_BUILDDIR)/.config
	$(call log,info,Building Buildroot)
	$(MAKE) $(BUILDROOT_MAKEFLAGS) $(BUILDROOT_TARGET)

# Useful configuration target. This is meant as a development helper to keep
# the repository configuration in sync with what the user is doing. It
# automatically replaces the earlier specified configuration file in the
# BR2_EXTERNAL directory.

.PHONY: buildroot-configure
buildroot-configure: $(BUILDROOT_BUILDDIR)/.config
	$(call log,info,Configuring Buildroot)
	$(MAKE) $(BUILDROOT_MAKEFLAGS) menuconfig
	$(call log,debug,Saving new defconfig)
	$(MAKE) $(BUILDROOT_MAKEFLAGS) savedefconfig

#################
## Run targets ##
#################

QEMU_PORT       ?= 9821
QEMU_DBG_PORT   ?= $(shell echo $$(( $(QEMU_PORT) + 1)) )
QEMU_DEBUG      := -gdb tcp::$(QEMU_DBG_PORT) -S

QEMU_MEM        ?= 2G
QEMU_SMP        ?= 4

QEMU_FLAGS := -m $(QEMU_MEM) -smp $(QEMU_SMP) -nographic \
                -machine virt,rom=$(BUILDROOT_BUILDDIR)/images/bootrom.bin \
                -bios $(BUILDROOT_BUILDDIR)/images/fw_jump.elf \
                -kernel $(BUILDROOT_BUILDDIR)/images/Image \
                -drive file=$(BUILDROOT_BUILDDIR)/images/rootfs.ext2,format=raw,id=hd0 \
                -device virtio-blk-device,drive=hd0 \
                -append "console=ttyS0 ro root=/dev/vda" \
                -netdev user,id=net0,net=192.168.100.1/24,dhcpstart=192.168.100.128,hostfwd=tcp::9821-:22 \
                -device virtio-net-device,netdev=net0 \
                -device virtio-rng-pci \

ifneq ($(KEYSTONE_DEBUG),)
	QEMU_FLAGS += $(QEMU_DEBUG)
endif

run-qemu:
	$(BUILDROOT_BUILDDIR)/host/bin/qemu-system-riscv64 $(QEMU_FLAGS)

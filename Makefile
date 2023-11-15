
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

export BUILDDIR                 ?= $(KEYSTONE)/build-$(KEYSTONE_PLATFORM)$(KEYSTONE_BITS)
export BUILDROOT_OVERLAYDIR     ?= $(BUILDDIR)/overlay
export BUILDROOT_BUILDDIR       ?= $(BUILDDIR)/buildroot.build

export KEYSTONE_PLATFORM        ?= generic
export KEYSTONE_BITS            ?= 64

include mkutils/args.mk
include mkutils/log.mk

BUILDROOT_CONFIGFILE    ?= qemu_riscv$(KEYSTONE_BITS)_virt_defconfig
ifeq ($(KEYSTONE_PLATFORM),mpfs)
	EXTERNALS += microchip
else ifeq ($(KEYSTONE_PLATFORM),unmatched)
    BUILDROOT_CONFIGFILE = riscv64_hifive_unmatched_defconfig
endif

# Highest priority external
EXTERNALS += keystone

BUILDROOT_MAKEFLAGS     := -C $(KEYSTONE_BUILDROOT) O=$(BUILDROOT_BUILDDIR)
BUILDROOT_MAKEFLAGS     += BR2_EXTERNAL=$(call SEPERATE_LIST,:,$(addprefix $(KEYSTONE_BR2_EXT)/,$(EXTERNALS)))

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

$(BUILDROOT_OVERLAYDIR): $(BUILDDIR)
	mkdir -p $@

# Configuration

$(BUILDROOT_BUILDDIR)/.config: $(BUILDROOT_BUILDDIR)
	$(call log,info,Configuring Buildroot with $(BUILDROOT_CONFIGFILE))
	$(MAKE) $(BUILDROOT_MAKEFLAGS) $(BUILDROOT_CONFIGFILE)
	echo "BR2_ROOTFS_OVERLAY=\"$(BUILDROOT_OVERLAYDIR)\"" >> $(BUILDROOT_BUILDDIR)/.config

# Overlay

$(BUILDROOT_OVERLAYDIR)/.done: $(BUILDROOT_OVERLAYDIR)
	$(call log,info,Setting up overlay)
	mkdir -p $(BUILDROOT_OVERLAYDIR)/root/.ssh
	ssh-keygen -C 'root@keystone' -t rsa -f $(BUILDROOT_OVERLAYDIR)/root/.ssh/id-rsa -N ''
	cp $(BUILDROOT_OVERLAYDIR)/root/.ssh/{id-rsa.pub,authorized_keys}
	touch $@

# Main build target for buildroot. The specific subtarget to build can be overriden
# by setting the BUILDROOT_TARGET environment variable.

BUILDROOT_TARGET        ?= all

.PHONY: buildroot
buildroot: $(BUILDROOT_BUILDDIR)/.config $(BUILDROOT_OVERLAYDIR)/.done
	$(call log,info,Building Buildroot)
	$(MAKE) $(BUILDROOT_MAKEFLAGS) $(BUILDROOT_TARGET) 2>&1 | \
            tee $(BUILDDIR)/build.log | LC_ALL=C grep -of scripts/grep.patterns

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
	sed -i '/BR2_ROOTFS_OVERLAY.*/d' $(KEYSTONE_BR2_EXT)/keystone/configs/$(BUILDROOT_CONFIGFILE)

.PHONY: linux-configure
linux-configure: $(BUILDROOT_BUILDDIR)/.config
	$(call log,info,Configuring Linux)
	$(MAKE) $(BUILDROOT_MAKEFLAGS) linux-menuconfig
	$(call log,debug,Saving new defconfig)
	$(MAKE) $(BUILDROOT_MAKEFLAGS) linux-savedefconfig
	LINUX_BUILDDIR=$$($(MAKE) -s KEYSTONE_LOG_LEVEL=$(LOG_FATAL) $(BUILDROOT_MAKEFLAGS) linux-show-info | jq -r '.linux|.build_dir') ; \
            LINUX_CONFIGFILE=$$(cat $(KEYSTONE_BR2_EXT)/keystone/configs/$(BUILDROOT_CONFIGFILE) | grep BR2_LINUX_KERNEL_CUSTOM_CONFIG_FILE | \
                                    awk -F'=' '{ print $$2 }' | sed 's;$$(BR2_EXTERNAL_KEYSTONE_PATH);$(KEYSTONE_BR2_EXT)/keystone;g' | tr -d '"'); \
            mv "$(BUILDROOT_BUILDDIR)/$$LINUX_BUILDDIR/defconfig" "$$LINUX_CONFIGFILE"

#################
## Run targets ##
#################

-include mkutils/plat/$(KEYSTONE_PLATFORM)/run.mk


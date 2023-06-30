################################################################################
#
# HSS
#
################################################################################

HSS_VERSION = v2023.06
HSS_SITE = $(call github,polarfire-soc,hart-software-services,$(HSS_VERSION))
HSS_DEPENDENCIES += keystone-sm

HSS_MAKE_OPTS += BOARD=mpfs-icicle-kit-es CROSS_COMPILE=riscv64-buildroot-linux-gnu- \
                   PLATFORM_CFLAGS="-fno-pic" PATH=$(BR_PATH) KEYSTONE_SM=$(KEYSTONE_SM_BUILDDIR)

define HSS_CONFIGURE_CMDS
	cp $(@D)/boards/mpfs-icicle-kit-es/def_config $(@D)/.config
	ln -sf $(STAGING_DIR)/usr/include/gnu/stubs-{lp64d.h,lp64.h}
endef

define HSS_BUILD_CMDS
	$(MAKE) $(HSS_MAKE_OPTS) -C $(@D)
endef

define HSS_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0644 -D $(@D)/Default/hss-envm-wrapper.bin $(BINARIES_DIR)/hss-envm-wrapper.bin
	$(INSTALL) -m 0644 -D $(@D)/Default/hss-l2scratch.bin $(BINARIES_DIR)/hss-l2scratch.bin
endef

$(eval $(generic-package))

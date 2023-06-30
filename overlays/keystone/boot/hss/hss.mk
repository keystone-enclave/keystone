################################################################################
#
# HSS
#
################################################################################

HSS_VERSION = v2023.06
HSS_SITE = $(call github,polarfire-soc,hart-software-services,$(HSS_VERSION))
HSS_DEPENDENCIES += keystone-sm

ifeq ($(BR2_TARGET_HSS),y)
ifeq ($(BR2_TARGET_HSS_SC_INSTALL_DIR),)
$(error BR2_TARGET_HSS_SC_INSTALL_DIR must be defined)
endif

ifeq ($(BR2_TARGET_HSS_FPGENPROG),)
$(error BR2_TARGET_HSS_FPGENPROG must be defined)
endif
endif

SOFTCONSOLE = $(BR2_TARGET_HSS_SC_INSTALL_DIR)
HSS_MAKE_OPTS += BOARD=mpfs-icicle-kit-es SC_INSTALL_DIR=$(SOFTCONSOLE) FPGENPROG=$(BR2_TARGET_HSS_FPGENPROG) \
                  PATH=$(SOFTCONSOLE)/riscv-unknown-elf-gcc/bin:$(BR_PATH) KEYSTONE_SM=$(KEYSTONE_SM_BUILDDIR)

define HSS_CONFIGURE_CMDS
	cp $(@D)/boards/mpfs-icicle-kit-es/def_config $(@D)/.config
endef

define HSS_BUILD_CMDS
	$(MAKE) $(HSS_MAKE_OPTS) -C $(@D)
endef

define HSS_INSTALL_TARGET_CMDS
	$(INSTALL) -m 0644 -D $(@D)/Default/hss-envm-wrapper.bin $(BINARIES_DIR)/hss-envm-wrapper.bin
	$(INSTALL) -m 0644 -D $(@D)/Default/hss-l2scratch.bin $(BINARIES_DIR)/hss-l2scratch.bin
endef

$(eval $(generic-package))

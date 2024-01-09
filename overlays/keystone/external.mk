
##############
## Packages ##
##############

# Bootloaders
include $(sort $(wildcard $(BR2_EXTERNAL_KEYSTONE_PATH)/boot/*/*.mk))

# Packages
include $(sort $(wildcard $(BR2_EXTERNAL_KEYSTONE_PATH)/package/*/*.mk))

# U-Boot
ifeq ($(KEYSTONE_PLATFORM),hifive_unmatched)

define UBOOT_COPY_HIFIVE_SOURCES
	cp -ar $(KEYSTONE)/overlays/keystone/board/sifive/hifive-unmatched/src/uboot/keystone $(@D)/arch/riscv/lib
	cp -ar $(KEYSTONE)/overlays/keystone/board/sifive/hifive-unmatched/src/uboot/keystone.h $(@D)/arch/riscv/include/asm
	cp -ar $(KEYSTONE)/overlays/keystone/board/sifive/hifive-unmatched/src/uboot/u-boot-spl-sanctum.lds $(@D)/arch/riscv/cpu
endef

UBOOT_POST_EXTRACT_HOOKS += UBOOT_COPY_HIFIVE_SOURCES

endif

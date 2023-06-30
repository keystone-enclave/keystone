
##############
## Packages ##
##############

# Bootloaders
include $(sort $(wildcard $(BR2_EXTERNAL_KEYSTONE_PATH)/boot/*/*.mk))

# Packages
include $(sort $(wildcard $(BR2_EXTERNAL_KEYSTONE_PATH)/package/*/*.mk))

######################
## Platform patches ##
######################

## unmatched ##
ifeq ($(KEYSTONE_PLATFORM),unmatched)

# U-Boot
define UBOOT_COPY_HIFIVE_SOURCES
	cp -ar $(KEYSTONE)/overlays/keystone/board/sifive/hifive-unmatched/src/uboot/keystone $(@D)/arch/riscv/lib
	cp -ar $(KEYSTONE)/overlays/keystone/board/sifive/hifive-unmatched/src/uboot/keystone.h $(@D)/arch/riscv/include/asm
	cp -ar $(KEYSTONE)/overlays/keystone/board/sifive/hifive-unmatched/src/uboot/u-boot-spl-sanctum.lds $(@D)/arch/riscv/cpu
endef
UBOOT_POST_EXTRACT_HOOKS += UBOOT_COPY_HIFIVE_SOURCES

endif

## mpfs ##
ifeq ($(KEYSTONE_PLATFORM),mpfs)

# post-image script
# pending https://lists.buildroot.org/pipermail/buildroot/2023-August/672838.html
target-post-image: keystone-fixup-per-package-paths
keystone-fixup-per-package-paths: host-finalize
ifeq ($(BR2_PER_PACKAGE_DIRECTORIES),y)
	$(Q)grep --binary-files=without-match -lrZ '$(PER_PACKAGE_DIR)/[^/]\+/host' $(HOST_DIR) \
	|while read -d '' f; do \
		file -b --mime-type "$${f}" | grep -q '^text/' || continue; \
		printf '%s\0' "$${f}"; \
	done \
	|xargs -0 --no-run-if-empty \
		$(SED) 's:$(PER_PACKAGE_DIR)/[^/]\+/host:$(HOST_DIR):g'
endif

# correct ldflags for hss-payload-generator
# pending https://github.com/linux4microchip/buildroot-external-microchip/pull/13
define HOST_HSS_PAYLOAD_GENERATOR_BUILD_CORRECTLY
	$(MAKE) -C $(@D)/tools/hss-payload-generator HOST_INCLUDES=-I$(HOST_DIR)/usr/include/ LDFLAGS="$(HOST_LDFLAGS)"
endef
HOST_HSS_PAYLOAD_GENERATOR_PRE_BUILD_HOOKS += HOST_HSS_PAYLOAD_GENERATOR_BUILD_CORRECTLY

# For secure device mapping, Keystone needs access to the device tree at
# OpenSBI boot time. However, Microchip's default infrastructure does not
# do this -- instead, the device tree is embedded into the U-boot FIT image.
# Therefore, we need to induce a dependency on Linux from the HSS (to make
# sure that the device tree is built in time), and then also copy the device
# tree to where the hss-payload-generator expects it to be.
HOST_HSS_PAYLOAD_GENERATOR_DEPENDENCIES += linux
$(HOST_HSS_PAYLOAD_GENERATOR_TARGET_CONFIGURE): linux-install

define HOST_HSS_PAYLOAD_GENERATOR_COPY_DTB
        cp $(BINARIES_DIR)/microchip/* $(@D)/tools/hss-payload-generator/
endef
HOST_HSS_PAYLOAD_GENERATOR_PRE_BUILD_HOOKS += HOST_HSS_PAYLOAD_GENERATOR_COPY_DTB

endif


##############
## Packages ##
##############

# Bootloaders
include $(sort $(wildcard $(BR2_EXTERNAL_KEYSTONE_PATH)/boot/*/*.mk))

# Packages
include $(sort $(wildcard $(BR2_EXTERNAL_KEYSTONE_PATH)/package/*/*.mk))

###############
## Overrides ##
###############

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

################################################################################
#
# Keystone SDK
#
################################################################################

ifeq ($(KEYSTONE_SDK),)
$(error KEYSTONE_SDK directory not defined)
endif

define HOST_KEYSTONE_SDK_EXTRACT_CMDS
	cp -ar $(KEYSTONE_SDK)/* $(@D)
	cp -ar $(KEYSTONE_SDK)/.post-install $(@D)
endef

HOST_KEYSTONE_SDK_CONF_OPTS += -DKEYSTONE_SDK_DIR=$(HOST_DIR)/usr/share/keystone/sdk
HOST_KEYSTONE_SDK_DEPENDENCIES += toolchain

$(eval $(host-cmake-package))

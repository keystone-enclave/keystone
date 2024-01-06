################################################################################
#
# Keystone SDK
#
################################################################################

ifeq ($(KEYSTONE_SDK),)
$(error KEYSTONE_SDK directory not defined)
else
include $(KEYSTONE)/mkutils/pkg-keystone.mk
endif

HOST_KEYSTONE_SDK_CONF_OPTS += -DKEYSTONE_SDK_DIR=$(HOST_DIR)/usr/share/keystone/sdk \
                                 -DKEYSTONE_BITS=${KEYSTONE_BITS}

HOST_KEYSTONE_SDK_DEPENDENCIES += toolchain

# Clean the examples too if we clean this package
host-keystone-sdk-dirclean: keystone-examples-dirclean

$(eval $(host-keystone-package))
$(eval $(host-cmake-package))

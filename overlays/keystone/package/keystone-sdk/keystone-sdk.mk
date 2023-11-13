################################################################################
#
# Keystone SDK
#
################################################################################

ifeq ($(KEYSTONE_SDK),)
$(error KEYSTONE_SDK directory not defined)
else
KEYSTONE_SDK_IGNORE_DIRS = */cmake-build-debug* */.idea*
include $(KEYSTONE)/mkutils/pkg-keystone.mk
endif

# Export the variable below for any other keystone packages to use
export KEYSTONE_SDK_DIR=$(HOST_DIR)/usr/share/keystone/sdk

HOST_KEYSTONE_SDK_CONF_OPTS += -DKEYSTONE_SDK_DIR=$(KEYSTONE_SDK_DIR) -DCMAKE_BUILD_TYPE=Debug
HOST_KEYSTONE_SDK_DEPENDENCIES += toolchain

# Clean dependant packages if we clean this one
host-keystone-sdk-dirclean: keystone-examples-dirclean \
                                keystone-sm-dirclean \
                                keystone-driver-dirclean

$(eval $(host-keystone-package))
$(eval $(host-cmake-package))

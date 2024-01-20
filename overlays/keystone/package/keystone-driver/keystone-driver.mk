################################################################################
#
# Keystone driver
#
################################################################################

ifeq ($(KEYSTONE_DRIVER),)
$(error KEYSTONE_DRIVER directory not defined)
else
include $(KEYSTONE)/mkutils/pkg-keystone.mk
endif

KEYSTONE_DRIVER_DEPENDENCIES += host-keystone-sdk

$(eval $(keystone-package))
$(eval $(kernel-module))
$(eval $(generic-package))

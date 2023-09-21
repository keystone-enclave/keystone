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

$(eval $(keystone-package))
$(eval $(kernel-module))
$(eval $(generic-package))

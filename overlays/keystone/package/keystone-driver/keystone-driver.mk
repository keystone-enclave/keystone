################################################################################
#
# Keystone driver
#
################################################################################

ifeq ($(KEYSTONE_DRIVER),)
$(error KEYSTONE_DRIVER directory not defined)
endif

define KEYSTONE_DRIVER_EXTRACT_CMDS
	cp -ar $(KEYSTONE_DRIVER)/* $(@D)
endef

$(eval $(kernel-module))
$(eval $(generic-package))

################################################################################
#
# vta-driver
#
################################################################################

ifeq ($(KEYSTONE_TVM),)
	$(error KEYSTONE_TVM directory not defined)
endif

define VTA_DRIVER_EXTRACT_CMDS
	cp -ar $(KEYSTONE_TVM)/vta_driver/* $(@D)
endef

$(eval $(kernel-module))
$(eval $(generic-package))

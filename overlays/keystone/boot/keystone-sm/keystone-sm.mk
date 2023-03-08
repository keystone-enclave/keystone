################################################################################
#
# SM
#
################################################################################

ifeq ($(KEYSTONE_SM),)
$(error KEYSTONE_SM directory not defined)
else
include $(KEYSTONE)/mkutils/pkg-keystone.mk
endif

# Make OpenSBI depend on this build, which depends on the SDK since it contains
# the shared headers which specify the communication protocol between the host
# <> kernel <> sm <> runtime <> eapp

OPENSBI_DEPENDENCIES += keystone-sm host-keystone-sdk
$(OPENSBI_TARGET_CONFIGURE): keystone-sm-install host-keystone-sdk-install

# Point OpenSBI at the correct location of the SM sources
OPENSBI_MAKE_ENV += PLATFORM_DIR=$(KEYSTONE_SM_BUILDDIR)/plat/

# Make keystone-sm dircleans also trigger opensbi-dirclean
keystone-sm-dirclean: opensbi-dirclean

$(eval $(keystone-package))
$(eval $(generic-package))

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

# Make OpenSBI depend on this build
OPENSBI_DEPENDENCIES += keystone-sm
$(OPENSBI_TARGET_CONFIGURE): keystone-sm-install

# Point OpenSBI at the correct location of the SM sources
OPENSBI_MAKE_ENV += PLATFORM_DIR=$(KEYSTONE_SM_BUILDDIR)/plat/

# For abi issue
OPENSBI_MAKE_ENV += PLATFORM_RISCV_TOOLCHAIN_DEFAULT=1

# Make keystone-sm dircleans also trigger opensbi-dirclean
keystone-sm-dirclean: opensbi-dirclean

$(eval $(keystone-package))
$(eval $(generic-package))

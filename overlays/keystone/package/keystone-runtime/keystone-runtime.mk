################################################################################
#
# Runtime
#
################################################################################

ifeq ($(KEYSTONE_RUNTIME),)
$(error KEYSTONE_RUNTIME directory not defined)
else
KEYSTONE_RUNTIME_IGNORE_DIRS = */cmake-build-debug* */.idea*
include $(KEYSTONE)/mkutils/pkg-keystone.mk
endif

# This is a bit of a dummy package, we only want it to copy its sources so that
# we can use it in the examples. Also, for change tracking as described in
# pkg-keystone.mk

# Clean the examples too if we clean this package
keystone-runtime-dirclean: keystone-examples-dirclean

$(eval $(keystone-package))
$(eval $(generic-package))

################################################################################
#
# Keystone examples
#
################################################################################

ifeq ($(KEYSTONE_EXAMPLES),)
$(error KEYSTONE_EXAMPLES directory not defined)
else
include $(KEYSTONE)/mkutils/pkg-keystone.mk
endif

KEYSTONE_EXAMPLES_DEPENDENCIES += host-keystone-sdk keystone-runtime opensbi
KEYSTONE_EXAMPLES_CONF_OPTS += -DKEYSTONE_SDK_DIR=$(HOST_DIR)/usr/share/keystone/sdk \
                                -DKEYSTONE_EYRIE_RUNTIME=$(KEYSTONE_RUNTIME_BUILDDIR) \
                                -DKEYSTONE_BITS=${KEYSTONE_BITS}
ifeq ($(KEYSTONE_PLATFORM),cva6)
KEYSTONE_EXAMPLES_CONF_OPTS += -Dfw_bin=$(BINARIES_DIR)/fw_payload.bin
endif

KEYSTONE_EXAMPLES_MAKE_ENV += KEYSTONE_SDK_DIR=$(HOST_DIR)/usr/share/keystone/sdk
KEYSTONE_EXAMPLES_MAKE_OPTS += examples

# Install only .ke files
define KEYSTONE_EXAMPLES_INSTALL_TARGET_CMDS
	find $(@D) -name '*.ke' | \
                xargs -i{} $(INSTALL) -D -m 755 -t $(TARGET_DIR)/usr/share/keystone/examples/ {}
endef

$(eval $(keystone-package))
$(eval $(cmake-package))


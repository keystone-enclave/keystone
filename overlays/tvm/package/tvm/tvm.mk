################################################################################
#
# tvm
#
################################################################################

ifeq ($(KEYSTONE_TVM),)
	$(error KEYSTONE_TVM directory not defined)
endif

TVM_VERSION = v0.12.0
TVM_SITE = https://github.com/apache/tvm
TVM_SITE_METHOD = git
TVM_GIT_SUBMODULES = YES
TVM_INSTALL_TARGET = NO
#HOST_TVM_DEPENDENCIES += host-llvm

TVM_CONF_OPTS = -DUSE_LIBBACKTRACE=off


TVM_MAKE_OPTS = tvm_runtime vta
HOST_TVM_MAKE_OPTS = tvm

HOST_TVM_CONF_OPTS = $(TVM_CONF_OPTS) \
			-DCMAKE_CXX_FLAGS="-I$(HOST_DIR)/usr/include"


#####################
## Configure hooks ##
#####################

# We need these for both the host and the target packages in order to get
# the compiler and runtime configurations, respectively, that we want

define TVM_CONFIGURE
	cp $(@D)/cmake/config.cmake $(@D)/config.cmake
	sed -i 's/USE_LIBBACKTRACE AUTO/USE_LIBBACKTRACE OFF/g' $(@D)/config.cmake
endef

define TVM_CONFIGURE_FOR_VTA
	sed -i 's/USE_VTA_FPGA OFF/USE_VTA_FPGA ON/g' $(@D)/config.cmake
	mkdir -p $(@D)/3rdparty/vta-hw/src/polarfire
	cp -ar $(KEYSTONE_TVM)/tvm_device/polarfire* \
            $(@D)/3rdparty/vta-hw/src/polarfire/
        ( cd $(@D)/3rdparty/vta-hw/config/ ; \
                cat vta_config.json | jq '.TARGET = "polarfire"' > vta_config.json.new ; \
                mv vta_config.json.new vta_config.json )
endef

# Pending correctly working llvm
define TVM_CONFIGURE_FOR_HOST
	sed -i 's/USE_LLVM OFF/USE_LLVM ON/g' $(@D)/config.cmake
endef

TVM_PRE_CONFIGURE_HOOKS += TVM_CONFIGURE TVM_CONFIGURE_FOR_VTA
HOST_TVM_PRE_CONFIGURE_HOOKS += $(TVM_PRE_CONFIGURE_HOOKS) #TVM_CONFIGURE_FOR_HOST

###################
## Install hooks ##
###################

# We want these only for the target build, which needs miscellaneous VTA configuration files.

define TVM_COPY_VTA_CONFIG
	mkdir -p $(TARGET_DIR)/usr/share/vta/config/
	chmod 755 $(TARGET_DIR)/usr/share/vta/config/
	cp -ar $(TVM_BUILDDIR)/3rdparty/vta-hw/config/* $(TARGET_DIR)/usr/share/vta/config/
endef

define TVM_COPY_LIBVTA
	$(INSTALL) -D -m 0755 $(@D)/libvta.so $(TARGET_DIR)/usr/lib/libvta.so
	$(INSTALL) -D -m 0755 $(@D)/libtvm_runtime.so $(TARGET_DIR)/usr/lib/libtvm_runtime.so
endef

TVM_POST_BUILD_HOOKS += TVM_COPY_VTA_CONFIG TVM_COPY_LIBVTA

$(eval $(cmake-package))
$(eval $(host-cmake-package))

################################################################################
#
# tvm
#
################################################################################

ifeq ($(KEYSTONE_TVM),)
	$(error KEYSTONE_TVM directory not defined)
endif

TVM_VERSION = v0.11.1
TVM_SITE = https://github.com/apache/tvm
TVM_SITE_METHOD = git
TVM_GIT_SUBMODULES = YES

TVM_CONF_OPTS = -DUSE_LIBBACKTRACE=off

define TVM_CONFIGURE_FOR_VTA
	cp $(@D)/cmake/config.cmake $(@D)/config.cmake
	sed -i 's/USE_VTA_FPGA OFF/USE_VTA_FPGA ON/g' $(@D)/config.cmake
	sed -i 's/USE_LIBBACKTRACE AUTO/USE_LIBBACKTRACE OFF/g' $(@D)/config.cmake
        ( cd $(@D)/3rdparty/vta-hw/config/ ; \
                cat vta_config.json | jq '.TARGET = "polarfire"' > vta_config.json.new ; \
                mv vta_config.json.new vta_config.json )
endef

define TVM_COPY_POLARFIRE_VTA
	mkdir -p $(@D)/3rdparty/vta-hw/src/polarfire
	cp -ar $(KEYSTONE_TVM)/tvm_device/*{.cc,.h} $(@D)/3rdparty/vta-hw/src/polarfire/
endef

TVM_PRE_CONFIGURE_HOOKS += TVM_CONFIGURE_FOR_VTA TVM_COPY_POLARFIRE_VTA

define TVM_COPY_VTA_CONFIG
	mkdir -p $(TARGET_DIR)/usr/share/vta/config/
	chmod 755 $(TARGET_DIR)/usr/share/vta/config/
	cp -ar $(TVM_BUILDDIR)/3rdparty/vta-hw/config/* $(TARGET_DIR)/usr/share/vta/config/
endef

define TVM_COPY_LIBVTA
	$(INSTALL) -D -m 0755 $(@D)/libvta.so $(TARGET_DIR)/usr/lib/libvta.so
endef

TVM_POST_INSTALL_TARGET_HOOKS += TVM_COPY_VTA_CONFIG TVM_COPY_LIBVTA

$(eval $(cmake-package))

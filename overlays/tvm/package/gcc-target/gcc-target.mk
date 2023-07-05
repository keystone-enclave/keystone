################################################################################
#
# gcc-target
#
################################################################################

GCC_TARGET_VERSION = $(GCC_VERSION)
GCC_TARGET_SITE = $(GCC_SITE)
GCC_TARGET_SOURCE = $(GCC_SOURCE)

# Use the same archive as gcc-initial and gcc-final
GCC_TARGET_DL_SUBDIR = gcc

GCC_TARGET_DEPENDENCIES = gmp mpfr mpc

# First, we use HOST_GCC_COMMON_MAKE_OPTS to get a lot of correct flags (such as
# the arch, abi, float support, etc.) which are based on the config used to
# build the internal toolchain
GCC_TARGET_CONF_OPTS = $(HOST_GCC_COMMON_CONF_OPTS)
# Then, we modify incorrect flags from HOST_GCC_COMMON_CONF_OPTS
GCC_TARGET_CONF_OPTS += \
	--with-sysroot=/ \
	--with-build-sysroot=$(STAGING_DIR) \
	--disable-__cxa_atexit \
	--with-gmp=$(STAGING_DIR) \
	--with-mpc=$(STAGING_DIR) \
	--with-mpfr=$(STAGING_DIR)
# Then, we force certain flags that may appear in HOST_GCC_COMMON_CONF_OPTS
GCC_TARGET_CONF_OPTS += \
	--disable-libquadmath \
	--disable-libsanitizer \
	--disable-plugin \
	--disable-lto
# Finally, we add some of our own flags
GCC_TARGET_CONF_OPTS += \
	--enable-languages=c,c++ \
	--disable-boostrap \
	--disable-libgomp \
	--disable-nls \
	--disable-libmpx \
	--disable-gcov \
	$(BR2_EXTRA_TARGET_GCC_CONFIG_OPTIONS)

GCC_TARGET_CONF_ENV = $(HOST_GCC_COMMON_CONF_ENV)

GCC_TARGET_MAKE_OPTS += $(HOST_GCC_COMMON_MAKE_OPTS)

# Install standard C headers (from glibc)
define GCC_TARGET_INSTALL_HEADERS
	cp -r $(STAGING_DIR)/usr/include $(TARGET_DIR)/usr
endef
GCC_TARGET_POST_INSTALL_TARGET_HOOKS += GCC_TARGET_INSTALL_HEADERS

# Install standard C libraries (from glibc)
GCC_TARGET_GLIBC_LIBS = \
	*crt*.o *_nonshared.a libc.a libpthread.a \
	libBrokenLocale.so libanl.so libbfd.so libc.so libcrypt.so libdl.so \
	libm.so libnss_compat.so libnss_db.so libnss_files.so libnss_hesiod.so \
	libpthread.so libresolv.so librt.so libthread_db.so libutil.so

define GCC_TARGET_INSTALL_LIBS
	for libpattern in $(GCC_TARGET_GLIBC_LIBS); do \
		$(call copy_toolchain_lib_root,$$libpattern) ; \
	done
endef
GCC_TARGET_POST_INSTALL_TARGET_HOOKS += GCC_TARGET_INSTALL_LIBS

# Remove unnecessary files (extra links to gcc binaries, and libgcc which is
# already in `/lib`)
define GCC_TARGET_RM_FILES
	rm -f $(TARGET_DIR)/usr/bin/$(ARCH)-buildroot-linux-gnu-gcc*
	rm -f $(TARGET_DIR)/usr/lib/libgcc_s*.so*
	rm -f $(TARGET_DIR)/usr/$(ARCH)-buildroot-linux-gnu/lib/ldscripts/elf32*
	rm -f $(TARGET_DIR)/usr/$(ARCH)-buildroot-linux-gnu/lib/ldscripts/elf64b*
endef
GCC_TARGET_POST_INSTALL_TARGET_HOOKS += GCC_TARGET_RM_FILES

# Rescue files which Buildroot would otherwise delete. See comment in external.mk
# for more information about this process.

define GCC_RESCUE_FILES
	mv $(TARGET_DIR)/usr/include $(TARGET_DIR)/usr/include.bak
	for f in $$(find $(TARGET_DIR)/lib/ $(TARGET_DIR)/usr/lib/ \
                        $(TARGET_DIR)/usr/libexec/ -name '*.a' ); \
                do mv $$f $$f.bak ; done
endef
GCC_TARGET_TARGET_FINALIZE_HOOKS += GCC_RESCUE_FILES

$(eval $(autotools-package))

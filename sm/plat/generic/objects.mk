# We include the default objects.mk
ifeq ($(PLATFORM),)
        $(error PLATFORM not defined for SM)
endif

include $(src_dir)/platform/$(PLATFORM)/objects.mk

# And then also define custom keystone SM functionality
# The header is only correctly included for VF2 if we use KEYSTONE_PLATFORM instead of PLATFORM here
ifeq ($(KEYSTONE_PLATFORM),starfive/visionfive2)
platform-genflags-y += "-DTARGET_PLATFORM_HEADER=\"platform/$(KEYSTONE_PLATFORM)/platform.h\""
platform-genflags-y += "-DBOARD_VISIONFIVE2"
else
platform-genflags-y += "-DTARGET_PLATFORM_HEADER=\"platform/$(PLATFORM)/platform.h\""
endif

include $(KEYSTONE_SM)/src/objects.mk
platform-objs-y += $(addprefix ../../src/,$(subst .c,.o,$(keystone-sm-sources)))

# We do need to make sure to include headers for the SM,
# since config.mk is no longer supported in newer OpenSBI versions
# we include the SM headers here
ifeq ($(KEYSTONE_PLATFORM),starfive/visionfive2)
ifeq ($(KEYSTONE_SM),)
$(error KEYSTONE_SM not defined for SM)
endif

ifeq ($(KEYSTONE_SDK_DIR),)
$(error KEYSTONE_SDK_DIR not defined)
endif

platform-cflags-y += -I$(KEYSTONE_SM)/src -I$(src_dir)/platform/$(PLATFORM)/include \
                        -I$(KEYSTONE_SDK_DIR)/include/shared
endif


ifeq ($(KEYSTONE_PLATFORM),generic)

carray-platform_override_modules-y += generic
platform-objs-y += generic.o

else ifeq ($(KEYSTONE_PLATFORM),hifive_unmatched)

include $(KEYSTONE_SM)/plat/hifive/unmatched/objects.mk

else ifeq ($(KEYSTONE_PLATFORM),starfive/visionfive2)

include $(KEYSTONE_SM)/plat/starfive/visionfive2/objects.mk

endif
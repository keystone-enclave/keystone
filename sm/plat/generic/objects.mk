# We include the default objects.mk
ifeq ($(PLATFORM),)
$(error PLATFORM not defined for SM)
endif

include $(src_dir)/platform/$(PLATFORM)/objects.mk

# And then also define custom keystone SM functionality
platform-genflags-y += "-DTARGET_PLATFORM_HEADER=\"platform/$(KEYSTONE_PLATFORM)/platform.h\""

include $(KEYSTONE_SM)/src/objects.mk
platform-objs-y += $(addprefix ../../src/,$(subst .c,.o,$(keystone-sm-sources)))

ifeq ($(KEYSTONE_PLATFORM),generic)

carray-platform_override_modules-y += generic
platform-objs-y += generic.o

else ifeq ($(KEYSTONE_PLATFORM),unmatched)

include $(KEYSTONE_SM)/plat/unmatched/objects.mk

endif

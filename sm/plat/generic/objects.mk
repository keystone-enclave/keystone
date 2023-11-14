# We include the default objects.mk

include $(src_dir)/platform/$(PLATFORM)/objects.mk

# And then also define custom keystone SM functionality
ifeq ($(PLATFORM),)
	$(error PLATFORM not defined for SM)
endif

platform-genflags-y += "-DTARGET_PLATFORM_HEADER=\"platform/$(KEYSTONE_PLATFORM)/platform.h\""

# Temporary fix for unmatched
platform-genflags-y += "-DTARGET_SMM_SIZE=0x80000"

include $(KEYSTONE_SM)/src/objects.mk
platform-objs-y += $(addprefix ../../src/,$(subst .c,.o,$(keystone-sm-sources)))

carray-platform_override_modules-y += generic
platform-objs-y += generic.o


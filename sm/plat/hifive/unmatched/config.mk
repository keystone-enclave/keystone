# Here, we just include the config.mk from the standard generic implementation
include $(src_dir)/platform/$(PLATFORM)/config.mk

# We do need to make sure to include headers for the SM
ifeq ($(KEYSTONE_SM),)
$(error KEYSTONE_SM not defined for SM)
endif

platform-cflags-y = -I$(KEYSTONE_SM)/src -I$(src_dir)/platform/$(PLATFORM)/include

include $(src_dir)/platform/$(PLATFORM)/config.mk

ifeq ($(KEYSTONE_SM),)
$(error KEYSTONE_SM not defined for SM)
endif

platform-cflags-y = -I$(KEYSTONE_SM)/src -I$(src_dir)/platform/$(PLATFORM)/include
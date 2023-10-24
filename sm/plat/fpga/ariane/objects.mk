#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (C) 2019 FORTH-ICS/CARV
#		Panagiotis Peristerakis <perister@ics.forth.gr>
#
include $(src_dir)/platform/$(PLATFORM)/objects.mk

# And then also define custom keystone SM functionality
ifeq ($(PLATFORM),)
	$(error PLATFORM not defined for SM)
endif

platform-genflags-y += "-DTARGET_PLATFORM_HEADER=\"platform/$(PLATFORM)/platform.h\""

include $(KEYSTONE_SM)/src/objects.mk
platform-objs-y += $(addprefix ../../../src/,$(subst .c,.o,$(keystone-sm-sources)))

carray-platform_override_modules-y += platform

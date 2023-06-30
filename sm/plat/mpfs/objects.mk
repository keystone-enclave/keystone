#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2019 Western Digital Corporation or its affiliates.
#
# Authors:
#   Atish Patra <atish.patra@wdc.com>
#

ifeq ($(KEYSTONE_SDK_DIR),)
$(error KEYSTONE_SDK_DIR not defined)
endif

# Define our platform
export PLATFORM=mpfs

# Ensure that standard SM crypto does not get built here
export KEYSTONE_SM_NO_CRYPTO=y
include $(KEYSTONE_SM)/src/objects.mk

platform-genflags-y += -I$(KEYSTONE_SM)/plat/$(PLATFORM) -I$(KEYSTONE_SM)/src \
                        -I$(KEYSTONE_SDK_DIR)/include/shared
platform-genflags-y += -DTARGET_PLATFORM_HEADER=\"platform/$(PLATFORM)/platform.h\"

platform-objs-y += $(addprefix $(KEYSTONE_SM)/src/,$(subst .c,.o,$(keystone-sm-sources)))
platform-objs-y += $(KEYSTONE_SM)/plat/$(PLATFORM)/crypto_interpose.o

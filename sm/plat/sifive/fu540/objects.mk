#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2019 Western Digital Corporation or its affiliates.
#
# Authors:
#   Atish Patra <atish.patra@wdc.com>
#
PLATFORM = sifive/fu540
KEYSTONE_SM_REL=../../../
platform-genflags-y += "-DTARGET_PLATFORM_HEADER=\"platform/$(PLATFORM)/platform.h\""

platform-objs-y += $(KEYSTONE_SM_REL)src/attest.o
platform-objs-y += $(KEYSTONE_SM_REL)src/cpu.o
platform-objs-y += $(KEYSTONE_SM_REL)src/crypto.o
platform-objs-y += $(KEYSTONE_SM_REL)src/enclave.o
platform-objs-y += $(KEYSTONE_SM_REL)src/pmp.o
platform-objs-y += $(KEYSTONE_SM_REL)src/sm.o
platform-objs-y += $(KEYSTONE_SM_REL)src/sm-sbi.o
platform-objs-y += $(KEYSTONE_SM_REL)src/sm-sbi-opensbi.o
platform-objs-y += $(KEYSTONE_SM_REL)src/thread.o
platform-objs-y += $(KEYSTONE_SM_REL)src/mprv.o
platform-objs-y += $(KEYSTONE_SM_REL)src/sbi_trap_hack.o
platform-objs-y += $(KEYSTONE_SM_REL)src/trap.o
platform-objs-y += $(KEYSTONE_SM_REL)src/ipi.o

platform-objs-y += $(KEYSTONE_SM_REL)src/sha3/sha3.o
platform-objs-y += $(KEYSTONE_SM_REL)src/ed25519/fe.o
platform-objs-y += $(KEYSTONE_SM_REL)src/ed25519/ge.o
platform-objs-y += $(KEYSTONE_SM_REL)src/ed25519/keypair.o
platform-objs-y += $(KEYSTONE_SM_REL)src/ed25519/sc.o
platform-objs-y += $(KEYSTONE_SM_REL)src/ed25519/sign.o

platform-objs-y += $(KEYSTONE_SM_REL)src/hkdf_sha3_512/hkdf_sha3_512.o
platform-objs-y += $(KEYSTONE_SM_REL)src/hmac_sha3/hmac_sha3.o

platform-objs-y += $(KEYSTONE_SM_REL)src/platform/$(PLATFORM)/platform.o

platform-objs-y += $(KEYSTONE_SM_REL)src/plugins/plugins.o

platform-objs-y += platform.o

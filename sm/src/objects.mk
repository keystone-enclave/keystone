#############
## Headers ##
#############

# General headers
keystone-sm-headers += sm_assert.h cpu.h enclave.h ipi.h mprv.h page.h platform-hook.h \
                        pmp.h safe_math_util.h sm.h sm-sbi.h sm-sbi-opensbi.h thread.h

# Crypto headers
ifneq ($(KEYSTONE_SM_NO_CRYPTO),y)
keystone-sm-headers += crypto.h ed25519/ed25519.h ed25519/fe.h ed25519/fixedint.h \
                        ed25519/ge.h ed25519/precomp_data.h ed25519/sc.h \
                        hkdf_sha3_512/hkdf_sha3_512.h hmac_sha3/hmac_sha3.h \
                        sha3/sha3.h
endif

# Platform headers
ifeq ($(KEYSTONE_PLATFORM),cva6)
    #for CVA6, the actual target platform is fpga/ariane, so PLATFORM variable is used
    keystone-sm-headers += platform/$(PLATFORM)/platform.h
else
    keystone-sm-headers += platform/$(KEYSTONE_PLATFORM)/platform.h
endif

ifeq ($(KEYSTONE_PLATFORM),sifive/fu540)
	keystone-sm-headers += platform/sifive/fu540/waymasks.h
endif

# Plugin headers
keystone-sm-headers += plugins/multimem.h plugins/plugins.h

##################
## Source files ##
##################

# Core files
keystone-sm-sources += attest.c cpu.c enclave.c pmp.c sm.c sm-sbi.c sm-sbi-opensbi.c \
                        thread.c mprv.c sbi_trap_hack.c trap.c ipi.c device.c

# Crypto
ifneq ($(KEYSTONE_SM_NO_CRYPTO),y)
keystone-sm-sources += crypto.c sha3/sha3.c ed25519/fe.c ed25519/ge.c ed25519/keypair.c \
                        ed25519/sc.c ed25519/sign.c hkdf_sha3_512/hkdf_sha3_512.c \
                        hmac_sha3/hmac_sha3.c
endif

# Platform
keystone-sm-sources += platform/$(PLATFORM)/platform.c

# Plugin files
keystone-sm-sources += plugins/multimem.c plugins/plugins.c

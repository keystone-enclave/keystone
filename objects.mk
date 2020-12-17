ifdef PLATFORM
  libsbiutils-genflags-y += "-DTARGET_PLATFORM_HEADER=\"platform/$(PLATFORM)/platform.h\""
else
	PLATFORM = "generic"
  libsbiutils-genflags-y += "-DTARGET_PLATFORM_HEADER=\"platform/generic/platform.h\""
endif

libsbiutils-genflags-y += -I$(CURDIR)/lib/utils/experimental/keystone

libsbiutils-objs-y += experimental/keystone/attest.o
libsbiutils-objs-y += experimental/keystone/cpu.o
libsbiutils-objs-y += experimental/keystone/crypto.o
libsbiutils-objs-y += experimental/keystone/enclave.o
libsbiutils-objs-y += experimental/keystone/pmp.o
libsbiutils-objs-y += experimental/keystone/sm.o
libsbiutils-objs-y += experimental/keystone/sm-sbi.o
libsbiutils-objs-y += experimental/keystone/sm-sbi-opensbi.o
libsbiutils-objs-y += experimental/keystone/thread.o
libsbiutils-objs-y += experimental/keystone/mprv.o
libsbiutils-objs-y += experimental/keystone/sbi_trap_hack.o
libsbiutils-objs-y += experimental/keystone/trap.o
libsbiutils-objs-y += experimental/keystone/ipi.o

libsbiutils-objs-y += experimental/keystone/sha3/sha3.o
libsbiutils-objs-y += experimental/keystone/ed25519/fe.o
libsbiutils-objs-y += experimental/keystone/ed25519/ge.o
libsbiutils-objs-y += experimental/keystone/ed25519/keypair.o
libsbiutils-objs-y += experimental/keystone/ed25519/sc.o
libsbiutils-objs-y += experimental/keystone/ed25519/sign.o

libsbiutils-objs-y += experimental/keystone/hkdf_sha3_512/hkdf_sha3_512.o
libsbiutils-objs-y += experimental/keystone/hmac_sha3/hmac_sha3.o

libsbiutils-objs-y += experimental/keystone/platform/$(PLATFORM)/platform.o

libsbiutils-objs-y += experimental/keystone/plugins/plugins.o

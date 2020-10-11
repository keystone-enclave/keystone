sm-objs-y = attest.o
sm-objs-y += cpu.o
sm-objs-y += crypto.o
sm-objs-y += enclave.o
sm-objs-y += pmp.o
sm-objs-y += sm.o
sm-objs-y += sm-sbi.o
sm-objs-y += sm-sbi-opensbi.o
sm-objs-y += thread.o

sm-objs-y += sha3/sha3.o
sm-objs-y += ed25519/fe.o
sm-objs-y += ed25519/ge.o
sm-objs-y += ed25519/keypair.o
sm-objs-y += ed25519/sc.o
sm-objs-y += ed25519/sign.o

sm-objs-y += platform/default/default.o

sm-objs-y += plugins/plugins.o

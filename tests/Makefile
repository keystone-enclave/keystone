CC = riscv64-unknown-linux-gnu-g++
OBJCOPY = riscv64-unknown-linux-gnu-objcopy

SDK_LIB_DIR =../lib
SDK_HOST_LIB = $(SDK_LIB_DIR)/libkeystone-host.a
SDK_EDGE_LIB = $(SDK_LIB_DIR)/libkeystone-edge.a
SDK_VERIFIER_LIB = $(SDK_LIB_DIR)/libkeystone-verifier.a

SDK_INCLUDE_HOST_DIR = $(SDK_LIB_DIR)/host/include
SDK_INCLUDE_EDGE_DIR = $(SDK_LIB_DIR)/edge/include
SDK_INCLUDE_VERIFIER_DIR = $(SDK_LIB_DIR)/verifier

RUNTIME=eyrie-rt
RUNNER=test-runner.riscv
CCFLAGS = -I$(SDK_INCLUDE_HOST_DIR) -I$(SDK_INCLUDE_EDGE_DIR) -I$(SDK_INCLUDE_VERIFIER_DIR) -std=c++11
LDFLAGS = -L$(SDK_LIB_DIR)

TESTS=stack fibonacci long-nop loop malloc fib-bench untrusted attestation

SRCS = $(patsubst %.riscv, %.cpp, $(RUNNER))
OBJS = $(patsubst %.riscv, %.o,$(RUNNER)) $(KEYSTONE_OBJ) edge_wrapper.o

all:  $(OBJS) $(SDK_HOST_LIB) $(SDK_EDGE_LIB) $(SDK_VERIFIER_LIB)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(RUNNER) $^
	$(foreach test, $(TESTS),\
		$(MAKE) -C $(test);\
	)
	rm -f test
	$(foreach test, $(TESTS), \
		echo "echo 'testing $(test)'" >> test;\
		echo "./$(RUNNER) $(test).eapp_riscv $(RUNTIME)" >> test; \
	)
	chmod +x test

$(SDK_HOST_LIB):
	$(MAKE) -C $(SDK_HOST_DIR)

$(OBJS): %.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o *.riscv test
	$(foreach test, $(TESTS), \
		$(MAKE) -C $(test) clean; \
	)

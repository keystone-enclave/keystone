CC := riscv64-unknown-linux-gnu-g++
OBJCOPY := riscv64-unknown-linux-gnu-objcopy

SDK_LIB_DIR := $(KEYSTONE_SDK_DIR)/lib
SDK_HOST_LIB := $(SDK_LIB_DIR)/libkeystone-host.a
SDK_EDGE_LIB := $(SDK_LIB_DIR)/libkeystone-edge.a
SDK_VERIFIER_LIB := $(SDK_LIB_DIR)/libkeystone-verifier.a

SDK_INCLUDE_HOST_DIR := $(SDK_LIB_DIR)/host/include
SDK_INCLUDE_EDGE_DIR := $(SDK_LIB_DIR)/edge/include
SDK_INCLUDE_VERIFIER_DIR := $(SDK_LIB_DIR)/verifier
SDK_INCLUDE_COMMON_DIR := $(KEYSTONE_SDK_DIR)/common/include
O := $(shell pwd)

RUNTIME := eyrie-rt
RUNNER := test-runner.riscv
CCFLAGS := -I$(SDK_INCLUDE_HOST_DIR) -I$(SDK_INCLUDE_EDGE_DIR) -I$(SDK_INCLUDE_VERIFIER_DIR) -I$(SDK_INCLUDE_COMMON_DIR) -std=c++11
LDFLAGS := -L$(SDK_LIB_DIR)

TESTS=stack fibonacci long-nop loop malloc fib-bench untrusted attestation data-sealing

OBJS := $(patsubst %.riscv, %.o,$(RUNNER)) $(KEYSTONE_OBJ) edge_wrapper.o
SRCS := $(patsubst %.o, %.cpp, $(OBJS))

DST_OBJS := $(addprefix $(O)/, $(OBJS))
DST_RUNNER := $(O)/$(RUNNER)
DST_TEST_SCRIPT := $(O)/test

.PHONY: all clean tests

all: $(DST_RUNNER) tests

tests $(DST_TEST_SCRIPT):
	$(foreach test, $(TESTS),\
		$(MAKE) -C $(test) O=$(O)/$(test) ;\
	)
	rm -f $(DST_TEST_SCRIPT)
	$(foreach test, $(TESTS), \
		echo "echo 'testing $(test)'"                    >> $(DST_TEST_SCRIPT) ;\
		echo "./$(RUNNER) $(test).eapp_riscv $(RUNTIME)" >> $(DST_TEST_SCRIPT) ;\
		echo '[ $$? -eq 0 ] || echo "... ERROR"'         >> $(DST_TEST_SCRIPT) ;\
	)
	chmod +x $(DST_TEST_SCRIPT)

$(DST_RUNNER): $(DST_OBJS) $(SDK_HOST_LIB) $(SDK_EDGE_LIB) $(SDK_VERIFIER_LIB)
	$(CC) $(CCFLAGS) $(LDFLAGS) -o $(DST_RUNNER) $^

$(SDK_HOST_LIB):
	$(MAKE) -C $(SDK_HOST_DIR)

$(DST_OBJS): $(O)/%.o: %.cpp
	@mkdir -p $(O)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(DST_OBJS) $(DST_RUNNER) $(DST_TEST_SCRIPT)
	$(foreach test, $(TESTS), \
		$(MAKE) -C $(test) O=$(O)/$(test) clean; \
	)
	rm -d $(O) 2>&1 || true

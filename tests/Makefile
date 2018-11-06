CC = riscv64-unknown-linux-gnu-g++
OBJCOPY = riscv64-unknown-linux-gnu-objcopy

SDK_HOST_DIR =../host
SDK_HOST_LIB = $(SDK_HOST_DIR)/libkeystone.a
SDK_INCLUDE_DIR = $(SDK_HOST_DIR)/include

RUNTIME=eyrie-rt
RUNNER=test-runner.riscv
CCFLAGS = -I$(SDK_INCLUDE_DIR)
LDFLAGS = -L$(SDK_HOST_DIR)

TESTS=c stack fibonacci long-nop loop malloc fib-bench

SRCS = $(patsubst %.riscv, %.cpp, $(RUNNER))
OBJS = $(patsubst %.riscv, %.o,$(RUNNER)) $(KEYSTONE_OBJ)

all: $(OBJS) $(SDK_HOST_LIB)
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
	make -C $(SDK_HOST_DIR)

$(OBJS): %.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

clean:
	rm -f *.o *.riscv test
	$(foreach test, $(TESTS), \
		$(MAKE) -C $(test) clean; \
	)

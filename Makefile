CC = riscv64-unknown-linux-gnu-g++
APP_DIR = apps/
SRC_DIR = src/
INCLUDE_DIR = include/
CCFLAGS = -I $(INCLUDE_DIR)
SRCS = keystone.cpp
APP_SRCS = hello.c hello_long.c
ENCLAVE_SRCS= simple_func
RUNTIME = keystone-rt

OBJS = $(patsubst %.cpp,%.o,$(SRCS))

BINS = $(patsubst %.c,%.riscv,$(APP_SRCS))

all: $(OBJS) $(BINS) $(RUNTIME)
	$(foreach enclave, $(addprefix $(APP_DIR), $(ENCLAVE_SRCS)),\
		$(MAKE) -C $(enclave) \
	)

%.o: $(addprefix $(SRC_DIR), %.cpp)
	$(CC) $(CCFLAGS) -c $<

%.riscv: $(addprefix $(APP_DIR), %.c) $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^

copy: $(BINS) 
	$(foreach bin, $^, \
		cd ..; ./copy_sdk_bin.sh $(bin)\
	)
	cd ..; ./copy_sdk_lib.sh $(RUNTIME)

$(RUNTIME):
	$(MAKE) -C runtime
	mv ./runtime/$(RUNTIME) ./

update-header:
	cp ../riscv-linux/arch/riscv/drivers/keystone_user.h ./include/keystone_user.h

clean:
	rm $(BINS) $(OBJS) $(RUNTIME)
	$(MAKE) -C runtime clean

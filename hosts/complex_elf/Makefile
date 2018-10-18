CC = riscv64-unknown-linux-gnu-g++
OBJCOPY = riscv64-unknown-linux-gnu-objcopy
CCFLAGS = -I../../include
APP_DIR = apps/
KEYSTONE_OBJ = ../../keystone.o

HOSTS = complex_elf.cpp
APPS = c_elf

OBJS = $(patsubst %.cpp,%.o,$(HOSTS))
BINS = $(patsubst %.cpp,%.riscv,$(HOSTS))
ENC_OBJS = $(patsubst %,%.eapp_riscv,$(APPS))

all: $(ENC_OBJS) $(OBJS) $(BINS)

%.eapp_riscv:
	make -C $(APP_DIR) $@

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $<

%.riscv: %.o $(KEYSTONE_OBJ)
	$(CC) $(CCFLAGS) -o $@ $^

copy: all
	cp $(BINS) $(patsubst %,$(APP_DIR)%,$(ENC_OBJS)) ../../bins/

clean:
	rm -f *.o *.riscv
	make -C $(APP_DIR) clean

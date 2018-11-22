CXX = riscv64-unknown-linux-gnu-g++
CC	= riscv64-unknown-liunux-gnu-gcc
AR = riscv64-unknown-linux-gnu-ar
#CXX = g++
#CC = gcc
#AR = ar
INCLUDE_DIR = ./
SRC_DIR = ./
CCFLAGS = -Wall -Werror -I $(INCLUDE_DIR) -std=gnu++11
CXXSRCS = json11.cpp keys.cpp report.cpp 
CSRCS = ed25519/fe.c \
				ed25519/ge.c \
				ed25519/sc.c \
				ed25519/verify.c \
				ed25519/sign.c \
				sha3/sha3.c 

CXXOBJS = $(patsubst %.cpp,%.o,$(CXXSRCS))
COBJS = $(patsubst %.c,%.o,$(CSRCS))

ARFLAGS = rcs

TARGET=../libkeystone-verifier.a

all: $(TARGET)
	#$(CXX) -std=gnu++11 test.cpp ../libkeystone-verifier.a -o test

$(TARGET): $(COBJS) $(CXXOBJS)
	$(AR) $(ARFLAGS) $@ $^

$(CXXOBJS): %.o: $(addprefix $(SRC_DIR)/, %.cpp)
	$(CXX) $(CCFLAGS) -c $<

$(COBJS): %.o: $(addprefix $(SRC_DIR)/, %.c)
	$(CXX) $(CCFLAGS) -c $< -o $@

clean:
	rm -f $(COBJS) $(CXXOBJS) $(TARGET)

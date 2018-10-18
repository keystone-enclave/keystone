CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -Wall -Werror -fPIC
LINK = riscv64-unknown-linux-gnu-ld
LINKFLAGS = -static
INCLUDE_DIR = ../include
AR = riscv64-unknown-elf-ar
ARFLAGS = rcs

ASM_SRCS = encret.s
C_SRCS =

A_OBJS = $(patsubst %.s,%.o,$(ASM_SRCS))
C_OBJS = $(patsubst %.c,%.o,$(C_SRCS))

all: libeapp_utils.a

encret.o: encret.s
	$(CC) -I$(INCLUDE_DIR) -c $<

libeapp_utils.a: $(A_OBJS) $(C_OBJS)
	$(AR) $(ARFLAGS) ../$@ $<

%.o: %.c
	$(CC) -I$(INCLUDE_DIR) -c $<

clean:
	rm -f *.o 
	rm ../libeapp_utils.a

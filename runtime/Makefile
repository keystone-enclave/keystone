CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -Wall -Werror
SRCS = runtime.c
ASM_SRCS = trampoline.S
RUNTIME = keystone-rt
LINK = riscv64-unknown-linux-gnu-ld

OBJS = $(patsubst %.c,%.o,$(SRCS))
ASM_OBJS = $(patsubst %.S,%.o,$(ASM_SRCS))

all: $(RUNTIME)

$(RUNTIME): $(ASM_OBJS) $(OBJS)
	$(LINK) -o $@ $^ -T runtime.lds

$(ASM_OBJS): $(ASM_SRCS)
	$(CC) $(CFLAGS) -c $< -fPIC

%.o: %.c
	$(CC) $(CFLAGS) -c $< -fPIC 

clean:
	rm -f $(RUNTIME) *.o

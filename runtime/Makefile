CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -Wall -Werror -fPIC
SRCS = page_fault.c mm.c
ASM_SRCS = entry.S
RUNTIME = keystone-rt
LINK = riscv64-unknown-linux-gnu-ld
LINKFLAGS = -static

OBJS = $(patsubst %.c,%.o,$(SRCS))
ASM_OBJS = $(patsubst %.S,%.o,$(ASM_SRCS))

all: $(RUNTIME)

$(RUNTIME): $(ASM_OBJS) $(OBJS)
	$(LINK) $(LINKFLAGS) -o $@ $^ -T runtime.lds

$(ASM_OBJS): $(ASM_SRCS)
	$(CC) $(CFLAGS) -c $<

%.o: %.c
	$(CC) $(CFLAGS) -c $< 

clean:
	rm -f $(RUNTIME) *.o

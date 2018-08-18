CC = riscv64-unknown-linux-gnu-gcc
CFLAGS = -Wall -Werror
SRCS = runtime.c
RUNTIME = keystone-rt
LINK = riscv64-unknown-linux-gnu-ld

OBJS = $(patsubst %.c,%.o,$(SRCS))

all: $(RUNTIME)

$(RUNTIME): $(OBJS)
	$(LINK) -o $@ $^ -T runtime.lds

%.o: %.c
	$(CC) $(CFLAGS) -c $< -fPIC 

clean:
	rm -f $(RUNTIME) *.o

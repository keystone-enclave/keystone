CC = riscv$(BITS)-unknown-linux-gnu-gcc
CFLAGS = -Wall -Werror -fPIC -I.

ASM_SRCS = uaccess.S
ASM_OBJS = $(patsubst %.S,%.o,$(ASM_SRCS))

BUILD_DEFS = -D__ASSEMBLY__ -include partial_linkage.h

COPY_HEADERS =
#uaccess.h

COPY_TARGETS = $(ASM_OBJS) $(COPY_HEADERS)

all: $(ASM_OBJS)
	cp $(COPY_TARGETS) ../

$(ASM_OBJS): $(ASM_SRCS)
	$(CC) $(CFLAGS) $(BUILD_DEFS) -c $<

clean:
	rm -f $(ASM_OBJS) $(patsubst %,../%,$(COPY_HEADERS))

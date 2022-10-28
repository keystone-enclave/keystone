CC = gcc
CFLAGS = -I../src -I../opensbi/include
FW_PATH ?= ../../build/sm.build/platform/generic/firmware
FW_ELF_PATH = $(FW_PATH)/fw_payload.elf
FW_BIN_PATH = $(FW_PATH)/fw_payload.bin
FW_SIZE = $(shell readelf --program-headers $(FW_ELF_PATH) | grep RWE | sed "s/^.*\(0x[0-9a-f]*\)[ \t]*\(RWE\).*$$/\1/")

all: hashgen

hashgen: sha3.o hash_generator.o
	$(CC) $(CFLAGS) -o $@ $^

sha3.o: ../src/sha3/sha3.c
	$(CC) -c $^ $(CFLAGS)

hash_generator.o: hash_generator.c
	$(CC) -c $^ $(CFLAGS)

hash: $(FW_ELF_PATH) $(FW_BIN_PATH) hashgen
	./hashgen $(FW_BIN_PATH) $(FW_SIZE) > sm_expected_hash.h

clean:
	rm -f *.o hashgen sm_expected_hash.h

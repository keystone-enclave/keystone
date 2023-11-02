#!/bin/bash

LOADER_SOURCE_DIR=`dirname $0`
BITS="64"

duplicated_files=(
    "../include/mm/common.h"
    "../tmplib/asm/csr.h"
    "../include/loader/elf.h"
    "../include/loader/elf32.h"
    "../include/loader/elf64.h"
    "../include/call/sbi.h"
    "../include/util/string.h"
    "../loader/elf.c"
    "../loader/elf32.c"
    "../loader/elf64.c"
    "../call/sbi.c"
    "../util/string.c"
)

# for robustness
for file in "${duplicated_files[@]}"; do
    rm "$(basename "$file")" || true
done

for file in "${duplicated_files[@]}"; do
    cp "$file" .
done

# TODOs:
# loader: need to combine functionality
# mm: same
# printf: replace seems unsafe??
# vm_defs: recombine? why was stuff removed?
# vm: also questionable

python3 flatten_includes.py

export BITS
make -C $LOADER_SOURCE_DIR clean
make -C $LOADER_SOURCE_DIR

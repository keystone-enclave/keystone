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
    "../include/util/printf.h"
    "../include/mm/vm_defs.h"
    "../include/mm/vm.h"
    "../include/mm/mm.h"
    "../include/loader/loader.h"
    "../loader/elf.c"
    "../loader/elf32.c"
    "../loader/elf64.c"
    "../call/sbi.c"
    "../util/string.c"
    "../util/printf.c"
    "../mm/vm.c"
    "../mm/mm.c"
    "../loader/loader.c"
)

for file in "${duplicated_files[@]}"; do
    cp "$file" .
done

python3 flatten_includes.py

export BITS
make -C $LOADER_SOURCE_DIR clean
make -C $LOADER_SOURCE_DIR

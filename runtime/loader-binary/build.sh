#!/bin/bash

LOADER_SOURCE_DIR=`dirname $0`
BITS="64"

duplicated_files=(
    "../loader/elf.c"
    "../loader/elf32.c"
    "../loader/elf64.c"
    "../call/sbi.c"
    "../util/string.c"
    "../util/printf.c"
    "../mm/vm.c"
    "../mm/mm.c"
    "../loader/loader.c"
    "../mm/freemem_ld.c"
)

for file in "${duplicated_files[@]}"; do
    cp "$file" .
done

export BITS
make -C $LOADER_SOURCE_DIR clean
make -C $LOADER_SOURCE_DIR

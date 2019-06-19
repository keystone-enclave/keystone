#!/bin/bash

#### Replace the variables ###

NAME=tests
VAULT_DIR=`dirname $0`
OUTPUT_DIR=$KEYSTONE_SDK_DIR/../buildroot_overlay/root/$NAME
EYRIE_DIR=$KEYSTONE_SDK_DIR/rts/eyrie
EYRIE_PLUGINS="freemem"
PACKAGE_FILES="stack/stack.eapp_riscv \
               fibonacci/fibonacci.eapp_riscv \
               long-nop/long-nop.eapp_riscv \
               loop/loop.eapp_riscv \
               malloc/malloc.eapp_riscv \
               fib-bench/fib-bench.eapp_riscv \
               untrusted/untrusted.eapp_riscv \
               attestation/attestation.eapp_riscv \
               test-runner.riscv \
               test \
               $EYRIE_DIR/eyrie-rt"
PACKAGE_SCRIPT="./test"

##############################


# create a build directory
mkdir -p $OUTPUT_DIR
OUTPUT_FILES_DIR=$OUTPUT_DIR/files
mkdir -p $OUTPUT_FILES_DIR

# build eyrie runtime
$EYRIE_DIR/build.sh $EYRIE_PLUGINS

# build the app
pushd $VAULT_DIR
make
for output in $PACKAGE_FILES; do
  cp $output $OUTPUT_FILES_DIR
done
popd

# package the enclave

makeself "$OUTPUT_FILES_DIR" "$OUTPUT_DIR/$NAME.ke" "Keystone vault archive" "$PACKAGE_SCRIPT"

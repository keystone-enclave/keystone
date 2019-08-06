#!/bin/bash

set -e

################################################################
#                   Replace the variables                      #
################################################################
NAME=tests
VAULT_DIR=`dirname $0`
BUILD_COMMAND=make
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

################################################################
#                       Sanity Check                           #
################################################################

# check if KEYSTONE_SDK_DIR is set
if [[ $KEYSTONE_SDK_DIR = "" ]]; then
  echo "KEYSTONE_SDK_DIR is not set"
  exit 1
fi

if [[ ! -d $KEYSTONE_SDK_DIR ]]; then
  echo "Invalid KEYSTONE_SDK_DIR '$KEYSTONE_SDK_DIR'"
  exit 1
fi

# check if riscv tools are in PATH
if ! (
  $(command -v riscv64-unknown-elf-g++ > /dev/null) &&
  $(command -v riscv64-unknown-linux-gnu-g++ > /dev/null) &&
  $(command -v riscv64-unknown-elf-gcc > /dev/null) &&
  $(command -v riscv64-unknown-linux-gnu-gcc > /dev/null)
  ); then
  echo "riscv tools are not in PATH"
  exit 1
fi

# check if OUTPUT_DIR is set
if [[ $OUTPUT_DIR = "" ]]; then
  echo "OUTPUT_DIR is not set"
  exit 1
fi

# check if EYRIE_DIR is valid
if [[ ! -d $EYRIE_DIR ]]; then
  echo "Invalid EYRIE_DIR '$EYRIE_DIR'"
  exit 1
fi

################################################################
#                       Build Enclave                          #
################################################################

# create a build directory
OUTPUT_FILES_DIR=$OUTPUT_DIR/files
mkdir -p $OUTPUT_FILES_DIR

# build eyrie runtime
$EYRIE_DIR/build.sh $EYRIE_PLUGINS

# build the app
pushd $VAULT_DIR
$BUILD_COMMAND
for output in $PACKAGE_FILES; do
  cp $output $OUTPUT_FILES_DIR
done
popd

# create vault archive & remove output files
makeself --noprogress "$OUTPUT_FILES_DIR" "$OUTPUT_DIR/$NAME.ke" "Keystone vault archive" $PACKAGE_SCRIPT
rm -rf $OUTPUT_FILES_DIR

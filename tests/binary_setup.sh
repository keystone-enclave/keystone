#!/usr/bin/env bash
set -e
################################################################
#                   Replace the variables                      #
################################################################
NAME=tests
VAULT_DIR=$(cd `dirname $0` && pwd)
BUILD_COMMAND=make
OUTPUT_DIR=$(pwd)
TEST_DIR=../../tests/tests/
EYRIE_DIR=../rts/eyrie
EYRIE_PLUGINS="freemem"


################################################################
#                       Build Enclave                          #
################################################################


# create a build directory
OUTPUT_FILES_DIR=$OUTPUT_DIR/test_binary
mkdir -p $OUTPUT_FILES_DIR

# build eyrie runtime
$EYRIE_DIR/build.sh $EYRIE_PLUGINS

cp $EYRIE_DIR/eyrie-rt $OUTPUT_FILES_DIR
cp $TEST_DIR/stack/stack.eapp_riscv $OUTPUT_FILES_DIR


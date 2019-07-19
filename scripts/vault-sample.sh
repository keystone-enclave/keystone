#!/bin/bash

set -e

#### Replace the variables ###

NAME=my_enclave
OUTPUT_DIR=$KEYSTONE_SDK_DIR/../buildroot_overlay/root/$NAME
EYRIE_DIR=$KEYSTONE_SDK_DIR/rts/eyrie
EYRIE_PLUGINS="freemem"
PACKAGE_FILES="my_eapp.bin my_host_app.bin $EYRIE_DIR/eyrie-rt"
PACKAGE_SCRIPT="./my_host_app.bin my_eapp.bin eyrie-rt"

##############################

VAULT_DIR=`dirname $0`

# create a build directory

mkdir -p $OUTPUT_DIR
OUTPUT_FILES_DIR=$OUTPUT_DIR/files
mkdir -p $OUTPUT_FILES_DIR

# build eyrie runtime

$EYRIE_DIR/build.sh $OUTPUT_FILES_DIR $EYRIE_PLUGINS

# build the app

make
for output in $PACKAGE_FILES; do
  cp $VAULT_DIR/$output $OUTPUT_FILES_DIR
done

# package the enclave

makeself "$OUTPUT_FILES_DIR" "$OUTPUT_DIR/$NAME.ke" "Keystone vault archive" $PACKAGE_SCRIPT

#!/bin/bash

LOADER_SOURCE_DIR=`dirname $0`
BITS="64"

export BITS
make -C $LOADER_SOURCE_DIR clean
make -C $LOADER_SOURCE_DIR

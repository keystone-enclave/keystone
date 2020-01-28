#!/bin/bash
SCRIPTPATH="$( cd "$(dirname "$0")" ; pwd -P )"
export RISCV=$SCRIPTPATH/riscv
export PATH=$RISCV/bin:$PATH
export KEYSTONE_SDK_DIR=$SCRIPTPATH/sdk

#!/bin/bash

git submodule sync --recursive
git submodule update --init --recursive

mkdir riscv
export RISCV=$(pwd)/riscv
export PATH=$PATH:$RISCV/bin
cd riscv-gnu-toolchain
./configure --prefix=$RISCV
make && make linux
cd ..

# build tests in SDK
export KEYSTONE_SDK_DIR=$(pwd)/sdk
cd sdk
mkdir build
cd build
cmake .. -DOUTPUT_DIR=$(pwd)/../lib
make
make install
cd ../..
./sdk/scripts/init.sh --runtime eyrie --force

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
make -C sdk
./sdk/scripts/init.sh --runtime eyrie --force
./sdk/examples/tests/vault.sh

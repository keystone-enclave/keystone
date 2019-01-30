#!/bin/bash

git config --global submodule.riscv-gnu-toolchain.update none
git submodule update --init --recursive

git clone https://github.com/keystone-enclave/firesim-riscv-tools-prebuilt.git --depth 1
export RISCV=$(pwd)/riscv
export PATH=$PATH:$RISCV/bin

cd firesim-riscv-tools-prebuilt
./installrelease.sh > riscv-tools-install.log 
mv distrib ../riscv
cd ..

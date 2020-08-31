#!/bin/bash

set -e

git submodule sync --recursive
git submodule update --init --recursive

mkdir riscv
export RISCV=$(pwd)/riscv
export PATH=$PATH:$RISCV/bin
cd riscv-gnu-toolchain
./configure --prefix=$RISCV
make && make linux
cd ..

# build SDK if not present
if [ -z $KEYSTONE_SDK_DIR ]
then
  echo "KEYSTONE_SDK_DIR is not set. Installing from $(pwd)/sdk"
  export KEYSTONE_SDK_DIR=$(pwd)/sdk/build
  cd sdk
  mkdir -p build
  cd build
  cmake ..
  make
  make install
  cd ../..
fi

# update source.sh
sed "s|KEYSTONE_SDK_DIR=.*|KEYSTONE_SDK_DIR=$KEYSTONE_SDK_DIR|" -i source.sh

echo "RISC-V toolchain and Keystone SDK have been fully setup"

#!/bin/bash

build_dir="__BUILD_DIR__"
build_args="__BUILD_ARGS__"
branch="__BRANCH__"

# Set up 

git checkout "$branch"

echo 'export PATH=/keystone/riscv64/bin:$PATH' >> $BASH_ENV
export KEYSTONE_SDK_DIR=/keystone/sdk/build

# Build SDK

cd /keystone/sdk
rm -rf build
mkdir build
cd build
KEYSTONE_SDK_DIR=$(pwd) cmake ..
make install

# Build runtime

cd /keystone/runtime
mkdir -p "$build_dir"
cd "$build_dir"
cmake -DCMAKE_C_COMPILER=$(which riscv64-unknown-linux-gnu-gcc) \
  -DCMAKE_OBJCOPY=$(which riscv64-unknown-linux-gnu-objcopy) \
  "$build_args" ..
make -j$(nproc)


#!/bin/bash

set -e

if [ -z "$BITS" ]; then
  BITS=64
fi

echo "Fast Setup (RV$BITS)";

DIST=xenial

if [[ $(ldconfig -p | grep "libmpfr.so.6") ]]; then
  DIST=bionic
fi

if [ "$BITS" = "64" ]; then
  TOOLCHAIN_7Z_FILE=riscv-toolchain-lp64d-rv64gc-2021.01.$DIST.7z
else
  TOOLCHAIN_7Z_FILE=riscv-toolchain-ilp32d-rv32gc-2021.01.$DIST.7z
  SDK_FLAGS="-DRISCV32=y"
fi

echo "Starting..."
if ( $(command -v riscv$BITS-unknown-linux-gnu-gcc > /dev/null) &&
  $(command -v riscv$BITS-unknown-elf-gcc > /dev/null) )
then
  echo "RISCV tools are already installed"
else
  echo "Downloading Prebuilt RISC-V Toolchain... "

  export RISCV=$(pwd)/riscv$BITS
  export PATH=$PATH:$RISCV/bin

  if [ -f $TOOLCHAIN_7Z_FILE ]; then
    rm $TOOLCHAIN_7Z_FILE;
  fi

  wget https://keystone-enclave.eecs.berkeley.edu/files/$TOOLCHAIN_7Z_FILE

  # Check tool integrity
  echo "Verifying prebuilt toolchain integrity..."

  sha256sum -c .prebuilt_tools_shasums --status --ignore-missing

  if [[ $? != 0 ]]; then
    echo "Toolchain binary download incomplete or corrupted. You can build the toolchain locally or try again."
    exit 1
  fi

  echo "Extracting Toolchain"
  7za x -y $TOOLCHAIN_7Z_FILE -o./riscv$BITS

  echo "Toolchain has been installed in $RISCV"

  rm $TOOLCHAIN_7Z_FILE
fi

echo "Updating and cloning submodules, this may take a long time"
git config submodule.riscv-gnu-toolchain.update none

# shallow clone submodules ahead of time (Git must be > 2.11)
if [ ! -e linux/.git ]; then
  git clone --shallow-since=2020-05-15 https://github.com/torvalds/linux.git linux
fi
if [ ! -e buildroot/.git ]; then
  git clone --shallow-since=2020-04-15 https://github.com/buildroot/buildroot.git buildroot
fi
if [ ! -e qemu/.git ]; then
  git clone --shallow-since=2020-11-15 https://github.com/qemu/qemu.git qemu
fi

git submodule sync --recursive
git submodule update --init --recursive

# build SDK if not present
if [ ! -z $KEYSTONE_SDK_DIR ] && [ -e $KEYSTONE_SDK_DIR ]
then
  echo "KEYSTONE_SDK_DIR is set to $KEYSTONE_SDK_DIR and present. Skipping SDK installation."
else
  echo "KEYSTONE_SDK_DIR is not set or present. Installing from $(pwd)/sdk"
  export KEYSTONE_SDK_DIR=$(pwd)/sdk/build$BITS
  cd sdk
  mkdir -p build
  cd build
  cmake .. $SDK_FLAGS
  make
  make install
  cd ../..
fi

# update source.sh
GCC_PATH=$(which riscv$BITS-unknown-linux-gnu-gcc)
RISCV_DIR=$(dirname $(dirname $GCC_PATH))
echo "export RISCV=$RISCV_DIR" > ./source.sh
echo "export PATH=\$RISCV/bin:\$PATH" >> ./source.sh
echo "export KEYSTONE_SDK_DIR=$KEYSTONE_SDK_DIR" >> ./source.sh

echo "RISC-V toolchain and Keystone SDK have been fully setup"
echo ""
echo " * Notice: run the following command to update enviroment variables *"
echo ""
echo "           source ./source.sh"
echo ""

#!/bin/bash

set -e

echo "Starting..."
if ( $(command -v riscv64-unknown-linux-gnu-gcc > /dev/null) &&
  $(command -v riscv64-unknown-elf-gcc > /dev/null) )
then
  echo "RISCV tools are already installed"
else
  echo "Downloading Prebuilt RISC-V Toolchain... "

  # The 1.0 version expected libmpfr.so.4, modern Ubuntu has .6
  TOOL_VER=1.0
  if [[ $(ldconfig -p | grep "libmpfr.so.6") ]]; then
      echo "Downloading tools v2.0 (support for libmpfr.so.6)"
      TOOL_VER=2.0
  fi

  export RISCV=$(pwd)/riscv
  export PATH=$PATH:$RISCV/bin
  wget https://keystone-enclave.eecs.berkeley.edu/files/${TOOL_VER}.tar.gz

  # Check tool integrity
  echo "Verifying prebuilt toolchain integrity..."
  sha256sum -c .prebuilt_tools_shasums --status --ignore-missing
  if [[ $? != 0 ]]
  then
      echo "Toolchain binary download incomplete or corrupted. You can build the toolchain locally or try again."
      exit 1
  fi

  tar -xzvf ${TOOL_VER}.tar.gz
  cd firesim-riscv-tools-prebuilt-${TOOL_VER}
  ./installrelease.sh > riscv-tools-install.log
  mv distrib riscv
  cp -R riscv ../
  cd ..
  echo "Toolchain has been installed in $RISCV"
fi

echo "Updating and cloning submodules, this may take a long time"
git config submodule.riscv-gnu-toolchain.update none

# shallow clone submodules ahead of time (Git must be > 2.11)
if [ ! -d linux ]; then
  git clone --shallow-since=2019-09-14 https://github.com/torvalds/linux.git linux
fi
if [ ! -d buildroot ]; then
  git clone --shallow-since=2019-08-29 https://github.com/buildroot/buildroot.git buildroot
fi
if [ ! -d qemu ]; then
  git clone --shallow-since=2018-08-14 https://github.com/qemu/qemu.git qemu
fi

git submodule sync --recursive
git submodule update --init --recursive

# build tests in SDK
export KEYSTONE_SDK_DIR=$(pwd)/sdk
make -C sdk
./sdk/scripts/init.sh --runtime eyrie --force

echo "Keystone is fully setup! You can build everything and run the tests with 'make run-tests'"

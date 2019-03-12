#!/bin/bash

echo "Starting..."
if ( $(command -v riscv64-unknown-linux-gnu-gcc > /dev/null) &&
  $(command -v riscv64-unknown-elf-gcc > /dev/null) )
then
  echo "RISCV tools are already installed"
else
  echo "Downloading Prebuilt RISC-V Toolchain... "
  export RISCV=$(pwd)/riscv
  export PATH=$PATH:$RISCV/bin
  wget https://github.com/keystone-enclave/firesim-riscv-tools-prebuilt/archive/2.0.tar.gz
  tar -xzvf 2.0.tar.gz
  cd firesim-riscv-tools-prebuilt-2.0
  ./installrelease.sh > riscv-tools-install.log
  mv distrib riscv
  cp -R riscv ../
  cd ..
  echo "Toolchain has been installed in $RISCV"
fi

git config --global submodule.riscv-gnu-toolchain.update none
git submodule update --init --recursive

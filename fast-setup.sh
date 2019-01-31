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
  git clone https://github.com/keystone-enclave/firesim-riscv-tools-prebuilt.git --depth 1
  cd firesim-riscv-tools-prebuilt
  ./installrelease.sh > riscv-tools-install.log 
  mv distrib ../riscv
  cd ..
fi

git config --global submodule.riscv-gnu-toolchain.update none
git submodule update --init --recursive

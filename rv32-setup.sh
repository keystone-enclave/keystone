#!/bin/bash

set -e

echo "Starting RV32 Instllation..."

if ( $(command -v riscv32-unknown-linux-gnu-gcc > /dev/null) &&
  $(command -v riscv32-unknown-elf-gcc > /dev/null) )
then
  echo "RISCV32 tools are already installed"
else
  echo "Downloading Prebuilt RISC-V 32 Toolchain... "

  export RISCV32=$(pwd)/riscv32
  export PATH=$PATH:$RISCV32/bin
  wget https://keystone-enclave.eecs.berkeley.edu/files/rv32gc.tar.gz

  # Check tool integrity
  echo "Verifying prebuilt toolchain integrity..."
  sha256sum -c .prebuilt_tools_shasums --status --ignore-missing
  if [[ $? != 0 ]]
  then
      echo "Toolchain binary download incomplete or corrupted. You can build the toolchain locally or try again."
      exit 1
  fi

  tar -xzvf rv32gc.tar.gz
  mv riscv32gc riscv32
  rm rv32gc.tar.gz
  echo "Toolchain has been installed in $RISCV32"
fi

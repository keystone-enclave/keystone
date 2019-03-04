#!/bin/bash

export HOST_PORT=${HOST_PORT:="$((2000 + RANDOM % 3000))"}

# Launch QEMU test
screen -L -dmS qemu ./scripts/run-qemu.sh
sleep 10
./scripts/test-qemu.sh

diff output.log tests/test-qemu.expected.log
if [ $? -eq 0 ]
then
  echo "[PASS] output.log matches with the expected output"
  exit 0
else
  echo "[FAIL] output.log does not match with the expected output"
  exit 1
fi

#!/bin/bash

export HOST_PORT=${HOST_PORT:="$((3000 + RANDOM % 3000))"}

EXPECTED_LOG_DIR=$1

# Launch QEMU test
screen -L -dmS qemu ./scripts/run-qemu.sh
sleep 10
./scripts/test-qemu.sh

diff output.log $EXPECTED_LOG_DIR/test-qemu.expected.log
if [ $? -eq 0 ]
then
  echo "[PASS] output.log matches with the expected output"
  exit 0
else
  echo "[FAIL] output.log does not match with the expected output"
  exit 1
fi

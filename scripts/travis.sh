#!/bin/bash

# Launch QEMU test
screen -dmS qemu ./scripts/run-qemu.sh
sleep 10
./scripts/test-qemu.sh

diff output.log tests/test-qemu.expected.log
if [ $? -eq 1 ]
then
  echo "[FAIL] output.log does not match with the expected output"
  exit 1
else
  echo "[PASS] output.log matches with the expected output"
  exit 0
fi

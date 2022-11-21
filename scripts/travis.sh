#!/bin/bash

EXPECTED_LOG_DIR=$1

# Launch QEMU test
screen -L -dmS qemu ./scripts/run-qemu.sh
# TODO: check for connectivity before starting, instead of sleeping
sleep 20
./scripts/test-qemu.sh 2>&1 | grep -v "Warning: Permanently added" | tee output.log

diff output.log $EXPECTED_LOG_DIR/test-qemu.expected.log
if [ $? -eq 0 ]
then
  echo "[PASS] output.log matches with the expected output"
  exit 0
else
  echo "[FAIL] output.log does not match with the expected output"
  exit 1
fi

#!/bin/bash

export HOST_PORT=${HOST_PORT:="$((3000 + RANDOM % 3000))"}

# Launch QEMU test
screen -L -dmS qemu ./scripts/run-qemu.sh
sleep 10
./scripts/test-qemu.sh

# delete the line containing "Uncompressing ... XX % ..."
sed "s/^Uncompressing.*//g" -i output.log
sed "s/^Uncompressing.*//g" -i tests/test-qemu.expected.log

diff output.log tests/test-qemu.expected.log
if [ $? -eq 0 ]
then
  echo "[PASS] output.log matches with the expected output"
  exit 0
else
  echo "[FAIL] output.log does not match with the expected output"
  exit 1
fi

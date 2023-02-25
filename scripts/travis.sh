#!/bin/bash

EXPECTED_LOG_DIR=$1
MONITOR_SOCKET=qemu-monitor.sock
ATTEMPTS=10
ATTEMPT_DELAY=5

# Launch QEMU
screen -L -dmS qemu ./scripts/run-qemu.sh
sleep 1

# Verify VM is running before running tests - abort after 10 failed attempts
running=0
for ((i = 0; i < $ATTEMPTS; ++i)); do
    result=$((echo "info status" && sleep 1) | \
	    socat - unix-connect:qemu-monitor.sock | \
	    grep -oF "running")
    if [[ -z "$result" ]]; then
	attempt=$((i+1))
	echo "VM not yet running. (check $attempt/$ATTEMPTS)"
	[ $attempt -ne $ATTEMPTS ] && sleep $ATTEMPT_DELAY
    else
	echo "VM confirmed to be running. Proceeding to run test(s)."
	running=1
	break
    fi
done

# Clean up socket
rm $MONITOR_SOCKET

if [ $running -eq 0 ]; then
    echo "[FAIL] VM not confirmed as running after max of $ATTEMPTS attempts - aborting."
    exit 1
fi


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

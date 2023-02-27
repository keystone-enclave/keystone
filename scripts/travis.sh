#!/bin/bash

EXPECTED_LOG_DIR=$1
MONITOR_SOCKET=qemu-monitor.sock
ATTEMPTS=10
ATTEMPT_DELAY=5

# Launch QEMU
screen -L -dmS qemu ./scripts/run-qemu.sh
sleep 1

# Verify VM is running before running tests - abort after 10 failed attempts
for ((i = 0;;)); do
    result=$( (echo "info status" && sleep 1) | \
	    nc -U $MONITOR_SOCKET -w 2 | \
	    grep -oF "running")
    [[ -z "$result" ]] || break
    echo "VM not yet running. (check $((++i))/$ATTEMPTS)"
    if [[ $i -eq $ATTEMPTS ]]; then
	echo "[FAIL] VM not confirmed as running after max of $ATTEMPTS checks - aborting."
	exit 1
    fi
    sleep $ATTEMPT_DELAY
done

# Clean up socket
rm $MONITOR_SOCKET
echo "VM confirmed to be running. Proceeding to run test(s)."

./scripts/test-qemu.sh 2>&1 | grep -v "Warning: Permanently added" | tee output.log

diff output.log $EXPECTED_LOG_DIR/test-qemu.expected.log
if [ $? -eq 0 ]; then
    echo "[PASS] output.log matches with the expected output"
    exit 0
else
    echo "[FAIL] output.log does not match with the expected output"
    exit 1
fi

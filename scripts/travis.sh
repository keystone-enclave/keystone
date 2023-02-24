#!/bin/bash

EXPECTED_LOG_DIR=$1

# Launch QEMU test
screen -L -dmS qemu ./scripts/run-qemu.sh
sleep 5

# We check if the VM is running
attempts=10
attempt_delay=5
running=0
for ((i = 0; i < $attempts; ++i)); do
    #result=$(echo "info status" | socat - unix-connect:/qemu-monitor-socket | grep "VM Status: running")
    #result=$(echo "info status" | telnet 127.0.0.1 55555 | grep "VM Status: running")
    result=$((echo "info status" && sleep 1) | netcat -N 127.0.0.1 55555 | grep "VM Status: running")
    qemu_monitor_result=$((echo "info status" && sleep 1) | netcat -N 127.0.0.1 55555)
    #qemu_monitor_result=$(echo "info status" | socat - unix-connect:/qemu-monitor-socket)
    echo "Received from monitor: $qemu_monitor_result"
    if [ -n $result ]; then
	echo "VM not yet running. [check $((i + 1)) of $attempts - using delay of $attempt_delay seconds]"
	sleep $attempt_delay
    else
	echo "VM confirmed to be running. Proceeding to run test(s)."
	running=1
	break
    fi
done

if [ $running -eq 0 ]; then
    echo "[FAIL] VM not confirmed as running in max of $attempts attempts - aborting."
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

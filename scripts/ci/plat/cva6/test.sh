#!/bin/bash
set -e

# Source global test configuration file
. scripts/ci/test-setup.sh

if [[ -z "$CMD_LOGFILE" ]]; then
        echo "CMD_LOGFILE undefined"
        exit 1
fi

if [[ -z "$KEYSTONE_BITS" ]]; then
        echo "KEYSTONE_BITS undefined"
        exit 1
fi

get_platform_var BOARD_IP

###############
## Run tests ##
###############
set -x

# Fix permissions on the key
chmod 600 "build-cva6$KEYSTONE_BITS/buildroot.build/target/root/.ssh/id-rsa"

# Start the board
export KEYSTONE_PLATFORM=cva6
export KEYSTONE_IP="$BOARD_IP"

TTYDEV=$(find_tty 0)
start_record_tty "$TTYDEV" 115200 "$LOGFILE" cva6-tty
power_on

# TODO: check for connectivity instead of sleeping
sleep 300

export CALL_LOGFILE="$CMD_LOGFILE"
touch "$CALL_LOGFILE"

KEYSTONE_COMMAND="modprobe keystone-driver" make call
KEYSTONE_COMMAND="/usr/share/keystone/examples/tests.ke" make call
# TODO: attestation does not yet work in cva6
#KEYSTONE_COMMAND="/usr/share/keystone/examples/attestor.ke" make call

power_off
stop_record_tty cva6-tty
exit 0

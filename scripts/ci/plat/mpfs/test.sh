#!/bin/bash
set -e

# Source global test configuration file
. scripts/ci/test-setup.sh

if [[ -z "$CMD_LOGFILE" ]]; then
        echo "CMD_LOGFILE undefined"
        exit 1
fi

get_platform_var BOARD_IP

###############
## Run tests ##
###############
set -x

# Fix permissions on the key
chmod 600 build-mpfs64/buildroot.build/target/root/.ssh/id-rsa

# Start the board
export KEYSTONE_PLATFORM=mpfs
export KEYSTONE_BITS=64
export KEYSTONE_IP="$BOARD_IP"

TTYDEV=$(find_tty 1)
start_record_tty "$TTYDEV" 115200 "$LOGFILE" mpfs-tty
power_on

# TODO: check for connectivity instead of sleeping
sleep 60

export CALL_LOGFILE="$CMD_LOGFILE"
touch "$CALL_LOGFILE"

KEYSTONE_COMMAND="modprobe keystone-driver" make call
KEYSTONE_COMMAND="/usr/share/keystone/examples/tests.ke" make call
# TODO: attestation does not yet work in mpfs
#KEYSTONE_COMMAND="/usr/share/keystone/examples/attestor.ke" make call

power_off
stop_record_tty mpfs-tty
exit 0

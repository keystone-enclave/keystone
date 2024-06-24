#!/bin/bash
set -e

# Source global test configuration file
. scripts/ci/test-setup.sh

###########
## Flash ##
###########
set -x

power_on
make -C build-mpfs64/buildroot.build/build/hss-v2023.06 program 2>/dev/null > "$LOGFILE"
power_off

# Check if flashing was successful
[[ ! -z $(cat "$LOGFILE" | grep "mpfsBootmodeProgrammer completed successfully") ]]

###########
## Check ##
###########

TTYDEV=$(find_tty 0)
start_record_tty "$TTYDEV" 115200 "$LOGFILE" mpfs-tty
power_on ; sleep 30; power_off
stop_record_tty mpfs-tty

# At least the first hart should have started
[[ ! -z $(cat "$LOGFILE" | sed -e 's/\x1b\[[0-9;]*m//g' | grep "u54 State Change:  \[Running\]") ]]

exit 0

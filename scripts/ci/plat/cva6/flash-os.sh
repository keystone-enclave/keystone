#!/bin/bash
set -e

# Source global test configuration file
. scripts/ci/test-setup.sh

if [[ -z "$KEYSTONE_BITS" ]]; then
        echo "KEYSTONE_BITS undefined"
        exit 1
fi

OS_FILENAME="build-cva6$KEYSTONE_BITS/buildroot.build/images/uImage"
get_platform_var HOST_IP
get_platform_var BOARD_IP

###########
## Flash ##
###########
set -x

TTYDEV=$(find_tty 0)
touch "$LOGFILE"
start_record_tty "$TTYDEV" 115200 "$LOGFILE" cva6-tty

power_on
wait_for "Hit any key to stop autoboot"
echo 'a' > "$TTYDEV"

rm -f "$TFTP_DIR/uImage"
cp "$OS_FILENAME" "$TFTP_DIR/uImage"

# Configure TFTP
echo "setenv serverip $HOST_IP" > "$TTYDEV" ; sleep 1
echo "setenv ipaddr $BOARD_IP" > "$TTYDEV" ; sleep 1

echo "tftp uImage" > "$TTYDEV"
wait_for "=>"
echo "mmc write 80200000 100000 10000" > "$TTYDEV"
wait_for "=>"

stop_record_tty cva6-tty
power_off
exit 0

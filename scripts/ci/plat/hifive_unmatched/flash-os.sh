#!/bin/bash
set -e

# Source global test configuration file
. scripts/ci/test-setup.sh

OS_FILENAME="build-hifive_unmatched64/buildroot.build/images/sdcard.img"
get_platform_var HOST_IP
get_platform_var BOARD_IP

###########
## Flash ##
###########
set -x

TTYDEV=$(find_tty 1)
touch "$LOGFILE"
start_record_tty "$TTYDEV" 115200 "$LOGFILE" hfu-tty

power_on_btn
wait_for "Hit any key to stop autoboot"
echo 'a' > "$TTYDEV"

rm -rf "$TFTP_DIR/sdcard.img"
cp "$OS_FILENAME" "$TFTP_DIR/sdcard.img"

# Configure TFTP
echo "setenv serverip $HOST_IP" > "$TTYDEV" ; sleep 1
echo "setenv ipaddr $BOARD_IP" > "$TTYDEV" ; sleep 1

echo "tftp sdcard.img" > "$TTYDEV"
wait_for "=>"
echo "mmc write 80210000 0 100000" > "$TTYDEV"
wait_for "=>"

stop_record_tty hfu-tty
power_off_btn

# For some reason, the UART for this board does not like
# being closed and then opened again quickly. This leads to
# test failures later on, so we delay a bit extra.
sleep 5
exit 0

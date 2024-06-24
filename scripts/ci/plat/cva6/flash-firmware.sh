#!/bin/bash
set -e

# Source global test configuration file
. scripts/ci/test-setup.sh

if [[ -z "$KEYSTONE_BITS" ]]; then
	echo "KEYSTONE_BITS undefined"
	exit 1
fi

FIRMWARE_FILENAME="build-cva6$KEYSTONE_BITS/buildroot.build/images/fw_payload.bin"
get_platform_var HOST_IP
get_platform_var BOARD_IP

###########
## Flash ##
###########
set -x

## ROM phase
TTYDEV=$(find_tty 0)
touch "$LOGFILE"
start_record_tty "$TTYDEV" 115200 "$LOGFILE" cva6-tty

# Wait for bootrom break
power_on
wait_for "Hit any key to enter update mode"
echo -n 'a' > "$TTYDEV"
sleep 1

# Send size
FILESIZE=$(stat --printf="%s" "$FIRMWARE_FILENAME")
printf "0: %.8x" "$FILESIZE" | sed -E 's/0: (..)(..)(..)(..)/0: \4\3\2\1/' | xxd -r -g0 > "$TTYDEV"
sleep 1

# Send bytes
dd if="$FIRMWARE_FILENAME" of="$TTYDEV"

## Uboot phase (TFTP)

# Wait for uboot prompt and send firmware image
wait_for "Hit any key to stop autoboot"
echo 'a' > "$TTYDEV"

rm -f "$TFTP_DIR/fw_payload.bin"
cp "$FIRMWARE_FILENAME" "$TFTP_DIR/fw_payload.bin"

echo "setenv serverip $HOST_IP" > "$TTYDEV" ; sleep 1
echo "setenv ipaddr $BOARD_IP" > "$TTYDEV" ; sleep 1

echo "tftp fw_payload.bin" > "$TTYDEV"
wait_for "=>"
echo "mmc write 80200000 800 1800" > "$TTYDEV"
wait_for "=>"

stop_record_tty cva6-tty
power_off
exit 0

#!/bin/bash
set -e

# Source global test configuration file
. scripts/ci/test-setup.sh

###########
## Flash ##
###########
set -x

# Wait for the board to come up a bit. We'll hammer it with serial
# input to ensure that we halt the boot at HSS

TTYDEV=$(find_tty 0)
configure_tty "$TTYDEV" 115200

power_on
NOW=$(date +%s)

# Disable output when actually hammering cause this is spammy
set +x
while [[ $(( $(date +%s) - $NOW )) -lt 10 ]]; do echo 'a' > "$TTYDEV" ; done
set -x

# Board should have halted, kick it into flash update mode

echo "" > "$TTYDEV"
echo "usbdmsc" > "$TTYDEV"

# Wait a bit for the USB to connect then flash
sleep 10
FOUND_DEVICE=""
for d in /dev/sd? ; do
	if [[ ! -z $(udevadm info --query=all -n "$d" | grep -i polarfire) ]]; then
		FOUND_DEVICE="yes"
		dd if="build-mpfs64/buildroot.build/images/sdcard.img" of="$d" bs=4M oflag=direct
		break
	fi
done

power_off
[[ ! -z "$FOUND_DEVICE" ]]

exit 0

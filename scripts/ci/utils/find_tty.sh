#!/bin/bash
set -e

if [[ "$#" -ne 3 ]]; then
	echo "usage: find_tty.sh [idvendor] [idproduct] [index]"
	exit 1
fi

TTY_IDVENDOR="$1"; shift
TTY_IDPRODUCT="$1"; shift

# See if we can find the correct TTYs
TTYS=()

for f in /sys/class/tty/ttyUSB* ; do
        if [[ $(cat $f/../../../../idVendor) == "$TTY_IDVENDOR" ]] && \
                [[ $(cat $f/../../../../idProduct) == "$TTY_IDPRODUCT" ]]; then
                # This is one of the TTYs we are looking for
                TTYS+=("$(basename $f)")
        fi
done

if [[ "$1" -lt "${#TTYS[@]}" ]]; then
	echo "/dev/${TTYS[$1]}"
	exit 0
fi

exit 1


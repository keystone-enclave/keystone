#!/bin/bash
set -e

# Generic setup script sourced by all CI test scripts. This allows us to have
# convenient global configuration options which are always available in
# consumer scripts.

###################
## Configuration ##
###################

# Check if we were called from the right location
if [[ $(basename "$PWD") != "keystone" ]]; then
	echo "CI scripts must be called from base Keystone directory"
	exit 1
fi

# Check if we have the right environment
if [[ -z "$KEYSTONE_PLATFORM" ]]; then
	echo "KEYSTONE_PLATFORM undefined"
	exit 1
fi

if [[ -z "$LOGFILE" ]]; then
	echo "LOGFILE undefined"
	exit 1
fi

# Source global configuration
. scripts/ci/configs/global.sh

# Source runner-specific configuration
if [[ ! -f "scripts/ci/configs/$(hostname).sh" ]]; then
        echo "No configuration file for this machine"
        exit 1
fi
. "scripts/ci/configs/$(hostname).sh"


######################
## Useful functions ##
######################

# Utility for expanding variables of the form VAR_<plat>, where plat is one of
# the supported Keytone platforms. Lots of variables like these live in the CI
# configuration files.

function get_platform_var {
	if [[ "$#" -ne 1 ]]; then
		echo "usage: get_platform_var [varname]"
		exit 1
	fi

	_VARNAME="$1_$KEYSTONE_PLATFORM"
	export "$1"="${!_VARNAME}"
	unset _VARNAME

	if [[ -z "${!1}" ]]; then
		echo "Variable $1 not defined for platform $KEYSTONE_PLATFORM"
		exit 1
	fi
}

# Power functions

get_platform_var RELAY_ID

function power_on {
	./scripts/ci/utils/relay_power.py "$RELAY_SERIAL" "$RELAY_ID" on
}

function power_off {
	./scripts/ci/utils/relay_power.py "$RELAY_SERIAL" "$RELAY_ID" off
}

function power_on_btn {
	# Very temporarily close the power on circuit
	power_on ; power_off
}

function power_off_btn {
	# Simulate holding the power button to force off
	power_on ; sleep 3 ; power_off
}

# Serial functions

get_platform_var TTY_IDVENDOR
get_platform_var TTY_IDPRODUCT

function find_tty {
	if [[ "$#" -ne 1 ]]; then
		echo "usage: find_tty [index]"
		exit 1
	fi

	echo $(./scripts/ci/utils/find_tty.sh "$TTY_IDVENDOR" "$TTY_IDPRODUCT" "$1")
}

function configure_tty {
	if [[ "$#" -ne 2 ]]; then
		echo "usage: configure_tty [tty_file] [baud]"
		exit 1
	fi

	if [[ ! -c "$1" ]]; then
		echo "$1 is not a valid character device"
		exit 1
	fi

	stty raw -echo "$2" < "$1"
}
export -f configure_tty

function start_record_tty {
	if [[ "$#" -ne 4 ]]; then
		echo "usage: start_record_tty [tty_file] [baud] [output_filename] [id]"
		exit 1
	fi

	screen -L -dmS "$4" bash -c "configure_tty $1 $2 ; cat $1 > $3"
}

function stop_record_tty {
	if [[ "$#" -ne 1 ]]; then
		echo "usage: stop_record_tty [id]"
		exit 1
	fi

	screen -XS "$1" quit
	screen -wipe || true
}

function wait_for {
	if [[ "$#" -ne 1 ]]; then
		echo "usage: wait_for [pattern]"
		exit 1
	fi

	./scripts/ci/utils/wait_for.py "$LOGFILE" "$1"
}

#############
## Cleanup ##
#############

# Make sure we turn off the boards if we die early
function final_shutdown {
	if [[ $? -ne 0 ]]; then
		if [[ "$KEYSTONE_PLATFORM" == "hifive_unmatched" ]]; then
			power_off_btn
		else
			power_off
		fi
	fi
}

trap final_shutdown EXIT

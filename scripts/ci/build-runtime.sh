#!/bin/bash
set -e

# Arguments:
#	1. Runtime source directory
#	2. Platform (i.e. generic, mpfs, etc)
#	3. Bitness (i.e. 32, 64)
#	4... variable number of args with USE flags

if [[ "$#" -lt 3 ]]; then
	echo "usage: build-runtime.sh [src dir] [platform] [bits] [build args ...]"
	exit 1
fi

# Parse arguments
KEYSTONE_RUNTIME="$1" ; shift
KEYSTONE_PLATFORM="$1" ; shift
KEYSTONE_BITS="$1" ; shift
BUILD_UUID=$(uuid)

# Create build directory
mkdir -p "build-runtime-$BUILD_UUID"
cd "build-runtime-$BUILD_UUID" || exit 1

# Configure cmake
BUILDDIR="$KEYSTONE_RUNTIME/../build-$KEYSTONE_PLATFORM$KEYSTONE_BITS/buildroot.build"
export PATH="$BUILDDIR/host/bin:$PATH"

cmake "$@" \
	-DCMAKE_C_COMPILER="$(which riscv$KEYSTONE_BITS-buildroot-linux-gnu-gcc)" \
	-DCMAKE_OBJCOPY="$(which riscv$KEYSTONE_BITS-buildroot-linux-gnu-objcopy)" \
	-DKEYSTONE_SDK_DIR="$BUILDDIR/per-package/keystone-examples/host/usr/share/keystone/sdk" \
	../runtime

# Build
make -j$(( 2 * $(nproc) ))

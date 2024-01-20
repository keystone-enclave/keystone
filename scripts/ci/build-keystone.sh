#!/bin/bash
set -e

# Arguments
#	1. Platform (i.e. generic, mpfs, etc)
#	2. Bitness (i.e. 32, 64)
#	3. Buildroot download directory (lower)
#	4. Buildroot download directory (upper)
#	5. Buildroot ccache directory (lower)
#	6. Buildroot ccache directory (upper)

if [[ "$#" -lt 6 ]]; then
	echo "usage: build-keystone.sh [platform] [bits]"
	exit 1
fi

# Extract arguments
export KEYSTONE_PLATFORM="$1"; shift
export KEYSTONE_BITS="$1"; shift
export DL_LOWER="$1"; shift
export DL_UPPER="$1"; shift
export CCACHE_LOWER="$1"; shift
export CCACHE_UPPER="$1"; shift

export DL_WORK="$PWD/buildroot/dl-work"
export CCACHE_WORK="$PWD/buildroot-ccache-work"
BUILDSCRIPT=$(mktemp)

# Generate buildscript. Note that variable substitution happens *here* instead
# of at runtime, which is what we want.
cat <<EOF > "$BUILDSCRIPT"

mkdir -p "$PWD/buildroot/dl"
mkdir -p "$DL_WORK"
mount -t overlay overlay \
	-o lowerdir="$DL_LOWER",upperdir="$DL_UPPER",workdir="$DL_WORK" \
	"$PWD/buildroot/dl"

mkdir -p "$PWD/buildroot-ccache"
mkdir -p "$CCACHE_WORK"
mount -t overlay overlay \
	-o lowerdir="$CCACHE_LOWER",upperdir="$CCACHE_UPPER",workdir="$CCACHE_WORK" \
	"$PWD/buildroot-ccache"

BUILDROOT_CCACHE="$PWD/buildroot-ccache" make -j$(( 2 * $(nproc) ))

EOF

function finish {
	# Clean up, called on script exit
	rm -rf "$DL_WORK" "$CCACHE_WORK" "$BUILDSCRIPT"
}
trap finish EXIT

# Run the build with overlaid directories
unshare -cm --keep-caps bash "$BUILDSCRIPT"

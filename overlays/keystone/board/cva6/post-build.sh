#!/bin/sh

cp $BR2_EXTERNAL_KEYSTONE_PATH/board/cva6/cva6-sdk/rootfs/* $BUILDROOT_OVERLAYDIR/

# Install udev rules & systemd units

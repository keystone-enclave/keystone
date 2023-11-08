#!/bin/sh
# Copy extlinux config
mkdir -p $TARGET_DIR/boot/extlinux
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/sifive/hifive-unmatched/extlinux.conf $TARGET_DIR/boot/extlinux/extlinux.conf

# Install udev rules & systemd units
mkdir -p $TARGET_DIR/usr/lib/systemd/system
mkdir -p $TARGET_DIR/etc/udev/rules.d
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/sifive/hifive-unmatched/unmatched-systemd-units/*.timer $TARGET_DIR/usr/lib/systemd/system/
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/sifive/hifive-unmatched/unmatched-systemd-units/*.service $TARGET_DIR/usr/lib/systemd/system/
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/sifive/hifive-unmatched/unmatched-udev-rules/*.rules $TARGET_DIR/etc/udev/rules.d/

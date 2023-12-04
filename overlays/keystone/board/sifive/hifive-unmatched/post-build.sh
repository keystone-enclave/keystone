#!/bin/sh

FUSDK_VER="2023.08.00"

# Copy extlinux config
mkdir -p $TARGET_DIR/boot/extlinux
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/sifive/hifive-unmatched/extlinux.conf $TARGET_DIR/boot/extlinux/extlinux.conf

# Install udev rules & systemd units
mkdir -p $TARGET_DIR/usr/lib/systemd/system
mkdir -p $TARGET_DIR/etc/udev/rules.d
wget -P "${TARGET_DIR}/etc/udev/rules.d" "https://raw.githubusercontent.com/sifive/freedom-u-sdk/${FUSDK_VER}/recipes-sifive/unmatched-udev-rules/files/unmatched/99-pwm-leds.rules"
wget -P "${TARGET_DIR}/usr/lib/systemd/system" "https://raw.githubusercontent.com/sifive/freedom-u-sdk/${FUSDK_VER}/recipes-sifive/unmatched-systemd-units/files/led-bootstate-green.service"
wget -P "${TARGET_DIR}/usr/lib/systemd/system" "https://raw.githubusercontent.com/sifive/freedom-u-sdk/${FUSDK_VER}/recipes-sifive/unmatched-systemd-units/files/led-bootstate-green.timer"

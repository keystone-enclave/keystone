#!/bin/sh

# Copy extlinux config
mkdir -p $TARGET_DIR/boot/extlinux
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/starfive/visionfive2/extlinux.conf $TARGET_DIR/boot/extlinux/extlinux.conf

# Configure network interfaces
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/starfive/visionfive2/10-end0.network $TARGET_DIR/etc/systemd/network/10-end0.network
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/starfive/visionfive2/20-end1.network $TARGET_DIR/etc/systemd/network/20-end1.network

# Copy sshd config to enable ssh root login, password is "starfive"
cp $BR2_EXTERNAL_KEYSTONE_PATH/board/starfive/visionfive2/sshd_config $TARGET_DIR/etc/ssh/sshd_config

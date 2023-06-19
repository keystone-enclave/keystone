#!/bin/bash

set -e

if [ -z "$BITS" ]; then
  BITS=64
fi

echo "Fast Setup (RV$BITS)";

echo "Updating and cloning submodules, this may take a long time"
# shallow clone submodules ahead of time (Git must be > 2.11)
if [ ! -e linux/.git ]; then
  git clone --shallow-since=2020-05-15 https://github.com/torvalds/linux.git linux
fi
if [ ! -e buildroot/.git ]; then
  git clone --shallow-since=2020-04-15 https://github.com/buildroot/buildroot.git buildroot
fi

git submodule sync --recursive
git submodule update --init --recursive

#!/bin/bash

EYRIE_SOURCE_DIR=`dirname $0`
PLUGINS=${@:1}

OPTIONS_FLAGS=

# OPTIONS_FLAGS ?= -DIO_SYSCALL_WRAPPING -DLINUX_SYSCALL_WRAPPING -DUSE_FREEMEM -DENV_SETUP
# OPTIONS_FLAGS += -DDYN_ALLOCATION
# OPTIONS_FLAGS += -DINTERNAL_STRACE

for plugin in $PLUGINS; do
  case $plugin in
    freemem)
      OPTIONS_FLAGS+="-DUSE_FREEMEM "
      ;;
    untrusted_io_syscall)
      OPTIONS_FLAGS+="-DIO_SYSCALL_WRAPPING "
      ;;
    linux_syscall)
      OPTIONS_FLAGS+="-DLINUX_SYSCALL_WRAPPING "
      ;;
    env_setup)
      OPTIONS_FLAGS+="-DENV_SETUP "
      ;;
    *)
      echo "Unknown plugin '$plugin'. Skipping"
      ;;
  esac
done

export OPTIONS_FLAGS
make -C $EYRIE_SOURCE_DIR clean
make -C $EYRIE_SOURCE_DIR

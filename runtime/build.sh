#!/bin/bash

EYRIE_SOURCE_DIR=`dirname $0`
REQ_PLUGINS=${@:1}
OPTIONS_LOG=${EYRIE_SOURCE_DIR}/.options_log
BITS="64"

# Build known plugins
declare -A CFLAGS
declare -A LDFLAGS
CFLAGS[freemem]="-DUSE_FREEMEM "
CFLAGS[untrusted_io_syscall]="-DIO_SYSCALL_WRAPPING "
CFLAGS[linux_syscall]="-DLINUX_SYSCALL_WRAPPING "
CFLAGS[env_setup]="-DENV_SETUP "
CFLAGS[strace_debug]="-DINTERNAL_STRACE "
CFLAGS[paging]="-DUSE_PAGING -DUSE_FREEMEM "
CFLAGS[page_crypto]="-DPAGE_CRYPTO "
CFLAGS[page_hash]="-DPAGE_HASH "
CFLAGS[debug]="-DDEBUG "
CFLAGS[rv32]="-mabi=ilp32d -march=rv32imafdc -mcmodel=medany "
LDFLAGS[rv32]="-melf32lriscv_ilp32"
#CFLAGS[dynamic_resizing]="-DDYN_ALLOCATION "

OPTIONS_C_FLAGS=
OPTIONS_LD_FLAGS=

echo > $OPTIONS_LOG

for plugin in $REQ_PLUGINS; do
    if [[ ! ${CFLAGS[$plugin]+_} ]]; then
        echo "Unknown Eyrie plugin '$plugin'. Skipping"
    else
        OPTIONS_C_FLAGS+=${CFLAGS[$plugin]}
        OPTIONS_LD_FLAGS+=${LDFLAGS[$plugin]}
        echo -n "$plugin " >> $OPTIONS_LOG
    fi
done

export BITS
export OPTIONS_C_FLAGS
export OPTIONS_LD_FLAGS
make -C $EYRIE_SOURCE_DIR clean
make -C $EYRIE_SOURCE_DIR

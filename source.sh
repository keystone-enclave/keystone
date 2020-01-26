SCRIPT=`realpath $0`
CURDIR=`dirname $SCRIPT`
export RISCV=$CURDIR/riscv
export PATH=$RISCV/bin:$PATH
export KEYSTONE_SDK_DIR=$CURDIR/sdk

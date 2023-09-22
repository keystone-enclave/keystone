export TOOLCHAIN_7Z_FILE=riscv-toolchain-lp64d-rv64gc-2021.01.bionic.7z
wget https://keystone-enclave.eecs.berkeley.edu/files/$TOOLCHAIN_7Z_FILE
7za x -y $TOOLCHAIN_7Z_FILE -o./old-toolchain
rm $TOOLCHAIN_7Z_FILE

export RISCV=$(pwd)/old-toolchain
export PATH=$PATH:$RISCV/bin
echo "export RISCV=$RISCV_DIR" > ./source.sh
echo "export PATH=\$RISCV/bin:\$PATH" >> ./source.sh

echo "Remember to source ./source.sh !!!"

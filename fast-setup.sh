export TOOLCHAIN_7Z_FILE=riscv-toolchain-lp64d-rv64gc-2021.01.bionic.7z
wget https://keystone-enclave.eecs.berkeley.edu/files/$TOOLCHAIN_7Z_FILE
7za x -y $TOOLCHAIN_7Z_FILE -o./old-toolchain
rm $TOOLCHAIN_7Z_FILE

export RISCV=$(pwd)/old-toolchain
export PATH=$PATH:$RISCV/bin
echo "export RISCV=$RISCV_DIR" > ./source.sh
echo "export PATH=\$RISCV/bin:\$PATH" >> ./source.sh

echo "Remember to source ./source.sh !!!"

# INSTRUCTIONS for running
# ./fast-setup.sh
# source ./source.sh
# git submodule update --init --recursive --depth 1
# make -j 12 # more if available
# make run
# # find port in command from make # 9821
# scp -i build-generic64/overlay/root/.ssh/id-rsa -P 9821 build-generic64/buildroot.build/build/keystone-*/*.ko root@localhost:.
# scp -i build-generic64/overlay/root/.ssh/id-rsa -P 9821 build-generic64/buildroot.build/build/keystone-examples-*/*/*.ke root@localhost:.

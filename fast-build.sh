git clone https://github.com/keystone-enclave/firesim-riscv-tools-prebuilt.git --depth=1

export RISCV=$(pwd)/riscv
export PATH=$PATH:$RISCV/bin

cd firesim-riscv-tools-prebuilt
./installrelease.sh
mv distrib ../riscv
cd ..
rm -rf firesim-riscv-tools-prebuilt

cd busybear-linux
make
cd ..

./scripts/apply-patch.sh
cd riscv-qemu
./configure --target-list=riscv64-softmmu,riscv32-softmmu
make
cd ..

cd riscv-linux
cp ../busybear-linux/conf/linux.config .config
make ARCH=riscv olddefconfig
make ARCH=riscv vmlinux
cd ..

cd riscv-pk
mkdir build
cd build
../configure \
    --enable-logo \
    --host=riscv64-unknown-elf \
    --with-payload=../../riscv-linux/vmlinux \
    --enable-sm
make
cd ../..

cd bootrom
make
cd ..

cd linux-keystone-driver
make
make copy
cd ..

cd sdk
make
make copy-tests
cd ..


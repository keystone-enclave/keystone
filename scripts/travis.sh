mkdir riscv
export RISCV=$(pwd)/riscv
export PATH=$PATH:$RISCV/bin
cd riscv-gnu-toolchain
./configure --prefix=$RISCV
make && make linux
cd ..

source source.sh 

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

cd bootrom
make
cd ..

cd sdk
make
make copy-tests
cd ..


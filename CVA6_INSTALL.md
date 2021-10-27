
### Setup

```bash
git clone https://github.com/andreaskuster/keystone.git
cd keystone
git checkout dev-cva6-support
./fast-setup.sh
```


### Compile 

```
mkdir <build directory>
cd <build directory>

export BUILD_ROOT=<absolute path to build directory>

cmake .. -Dcva6=y
make

make buildroot
make linux
make sm
make bootrom
make driver
make tests

make image
```

### Flash SD Card

```
sudo sgdisk --clear --new=1:2048:67583 --new=2 --typecode=1:3000 --typecode=2:8300 /dev/sdc
sudo dd if=sm.build/platform/generic/firmware/fw_payload.bin  of=/dev/sdc1 status=progress oflag=sync bs=1M
```



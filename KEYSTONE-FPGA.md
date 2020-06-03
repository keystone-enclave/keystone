# Keystone on FPGA (currently only Xilinx vc707 is tested)

## Preparation

### Required Hardware

Download `U500 VC707 FPGA Getting Started Guide` from the link.
[https://www.sifive.com/documentation](https://www.sifive.com/documentation)

Refer section `Required Hardware` for preparing required hardware configuration of vc707 devkit.

### Install Vivado

Download Vivado 2016.04 from xilinx website and install it. The file to download is `Vivado HLx 2016.4: WebPACK and Editions`.
The newer version of Vivado was known to fail and we have not tested any later version than 2016.04.

During the installation, select `Vivado HL Design Edition`.

Acquire a single node-locked license for Vivado came with vc707 devket and install it from logging in to Xilinx website with your account.

Type `$ ifconfig -a` to make sure that the network interface name is `eth0`. If not, the Vivado cannot recognize the license from the NIC interface when it is similar to `enp0s25`.

Must follow the bellow to rename the network interface:

```sh
$ sudo vi /etc/default/grub
```

Then add this line beneath those GRUB... lines:

```
GRUB_CMDLINE_LINUX="net.ifnames=0 biosdevname=0"
```

Then update the grub:
```sh
$ sudo grub-mkconfig -o /boot/grub/grub.cfg
$ sudo update-grub
```

And reboot the machine.
Check with `ifconfig` again if `eth0` is shown or not.

### Install usb-serial driver

Go to the directory where driver is.

```sh
$ cd to [installation path]/Vivado/2016.4/data/xicom/cable_drivers/lin64/install_script/install_drivers
```
then run the script
```sh
$ sudo ./install_drivers
```

### Install required packages for building bitstream for vc707.

After installing Ubuntu/Debian

Do not forget updating all packages.
```sh
$ sudo apt update
$ sudo apt upgrade
```

Install required additional packages.
```sh
$ sudo apt-get install autoconf automake autotools-dev curl libmpc-dev libmpfr-dev libgmp-dev libusb-1.0-0-dev gawk build-essential bison flex texinfo gperf libtool patchutils bc zlib1g-dev device-tree-compiler pkg-config libexpat-dev python wget

$ sudo apt-get install default-jre
```

### Install sbt, varilator and scala which are required for building from Chisel

Build and install sbt.
```sh
$ echo "deb https://dl.bintray.com/sbt/debian /" | sudo tee -a /etc/apt/sources.list.d/sbt.list
$ sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv 642AC823
$ sudo apt-get update
$ sudo apt-get install sbt
```

Build and install varilator.
```sh
$ sudo apt-get install git make autoconf g++ flex bison
$ git clone http://git.veripool.org/git/verilator
$ cd verilator
$ git checkout -b verilator_3_922
$ unset VERILATOR_ROOT # For bash, unsetenv for csh
$ autoconf # To create ./configure script
$ ./configure
$ make -j `nproc`
$ sudo make install
```

Install scala
```sh
$ sudo apt install scala
```

### Toolchain

To compile the bootloaders for VC707
FPGA dev kits, the RISC-V toolchain must be installed locally and
set the ${RISCV} environment variable to point to the location of where the
toolchains are installed. You can build the toolchain from scratch
or download the tools `GNU Embedded Toolchain` here:
[https://www.sifive.com/products/tools/](https://www.sifive.com/products/tools/)

After installing toolchain and vivado, you must set the environment variables.

If you have installed toolchain to
`/home/riscv/riscv64-elf-tc/bin/riscv64-unknown-elf-gcc`
then run the following command. Do not include /bin at the end of the string.

```sh
$ export RISCV=/home/riscv/riscv64-elf-tc
```

In order to run the `mcs` target in the next step, it must have the `vivado`
executable on your `PATH`.

If vivado is installed to `/tools/Xilinx/Vivado/2016.4/bin`,
set the `PATH` with the following command.
```sh
$ export PATH=${PATH}:/opt/Xilinx/Vivado/2016.4/bin
```
Change the line above if the `vivado` is installed to
`/tool/Xilinx/Vivado/2016.4/bin` accordingly.


## Build bitstream for FPGA which are able to run Keystone

### Sources for FPGA

Git clone and checkout the SiFive freedom:

```sh
$ git clone https://github.com/sifive/freedom.git
$ cd freedom/
$ git checkout 37207f100227f4c34994db067b5fea3e383299ce
$ git submodule update --init --recursive
```

It will require to add bootrom ported on vc707 with keystone feature.
Git clone the keystone's zsbl+fsbl under the bootrom/

```sh
$ cd bootrom/
$ git submodule add https://github.com/keystone-enclave/freedom-u540-c000-bootloader.git
$ cd freedom-u540-c000-bootloader/
$ git checkout bbfcc288fb438312af51adef420aa444a0833452
$ cd ../../	#get back to the top folder of freedom
```

Change one line which is stating address of bootrom.

```sh
$ vi src/main/scala/unleashed/DevKitConfigs.scala
```

Modify from:

```scala
  case PeripheryMaskROMKey => List(
    MaskROMParams(address = 0x10000, name = "BootROM"))
```

To:

```scala
  case PeripheryMaskROMKey => List(
    MaskROMParams(address = 0x78000000, name = "BootROM"))
```

### Generate bitstream and bootloader

Set the riscv toolchain & vivado tool to the PATH done above:

```sh
$ export RISCV=/home/riscv/riscv64-elf-tc
$ export PATH=${PATH}:/opt/Xilinx/Vivado/2016.4/bin:${RISCV}/bin
```

From the top folder of freedom:

This is for generate the verilog file:
```sh
$ make BOOTROM_DIR=`pwd`/bootrom/freedom-u540-c000-bootloader -f Makefile.vc707-u500devkit verilog -j`nproc`
```

This is for generate the bitstream file for FPGA:
```sh
$ make BOOTROM_DIR=`pwd`/bootrom/freedom-u540-c000-bootloader -f Makefile.vc707-u500devkit mcs -j`nproc`
```

## Program the Board

Remember to switch the switches above the LCD to UP-UP-DOWN-UP-DOWN, then open vivado, open hardware manager, open target board, auto connect.


Vivado direct programming:

	a. Launch Vivado
	b. Open Hardware Manager
	c. Right-click on the xc7vx485t_0
	d. Program Device ...
	e. Select the .bit file in the builds/vc707-u500devkit/obj/VC707Shell.bit
	f. Program

Vivado flash programming:

	a. Launch Vivado
	b. Open Hardware Manager
	c. right-click on the xc7vx485t_0
	d. Add Configuration Memory Device ...
	e. select the one with the Alias of 28f00ag18f ---> OK
	f. OK to continue to program the device
	g. select the Configuration file (.mcs) and PRM file (.prm) in the builds/vc707-u500devkit/obj/
	h. select the RS pins: 25:24
	i. OK to write data to the flash
	j. finally, right-click again on the xc7vx485t_0 and select Boot from Configuration Memory Device

## Build Keystone for FPGA

### Prerequisites (Ubuntu/Debian)

Install packages for building Keystone.
```sh
$ sudo apt update
$ sudo apt install autoconf automake autotools-dev bc bison build-essential curl expat libexpat1-dev flex gawk gcc git gperf libgmp-dev libmpc-dev libmpfr-dev libtool texinfo tmux patchutils zlib1g-dev wget bzip2 patch vim-common lbzip2 python pkg-config libglib2.0-dev libpixman-1-dev device-tree-compiler expect libssl-dev
```

Packages for building qemu.
```sh
$ sudo apt install git libglib2.0-dev libfdt-dev libpixman-1-dev zlib1g-dev
```

### Sources

Clone keystone.

```sh
$ git clone https://github.com/keystone-enclave/keystone.git
$ cd keystone
$ git checkout 500bc779322e08da22caed4b2495733294d36541
```

### Setup and downloading toolchain.

Remove the toolchain from the PATH added in previous instruction, since it require different toolchains than building the bitstream.
The easiest way is to logout from Linux once.

Confirm that your PATH doesn't include any riscv-toolchain.

```sh
$ echo $PATH
```

Then run the setup script. The toolchains will be downloaded inside the script.
```sh
$ unset RISCV
$ ./fast-setup.sh
```

### Build

Add the toolchain downloaded by the script `fast-setup.sh` to the PATH.

```sh
$ export PATH=$(pwd)/riscv/bin:${PATH}
```

```sh
$ make -j `nproc`
$ cd ..
```

## Prepare keystone bootable SD Card

Choose a SD card which is larger than 8GB. The following example is a 8GB SD card.
The contents of the SD card will be destroyed.

### Partition the SD card

The SD card require to have special partition arrangement.

The partitions must be created with following customized gptfdisk tools. The equivalent gptfdisk tools in Linux distribution do not work.

```sh
$ git clone https://github.com/tmagik/gptfdisk.git
$ cd gptfdisk/
$ git checkout 3d6a15873f582803aa8ad3288b3e32d3daff9fde
$ make -j`nproc`
```

Start to create partitions in SD card as following configuration.

```
	Number	Start (sector)	End (sector)	Size		Code	Name
	1	2048		67583		32.0 MiB	5202	SiFive bare-metal (...
	2	264192		15759326	7.4 GiB		8300	Linux filesystem
	4	67584		67839		128.0 KiB	5201	SiFive FSBL (first-...
```

First it needs to find the location of the SD card.
The location of the SD card varies on all Linux machines and requires to find the location on your machine.

Type `lsblk` before inserting SD card to Linux machine.

```sh
$ lsblk
```
It will show something similar bellow.
```
NAME   MAJ:MIN RM   SIZE RO TYPE MOUNTPOINT
sda      8:0    0 223.6G  0 disk
├─sda1   8:1    0 212.9G  0 part /
├─sda2   8:2    0     1K  0 part
└─sda5   8:5    0  10.8G  0 part [SWAP]
```

Then insert the SD card and type `lsblk` again.
```sh
$ lsblk
NAME   MAJ:MIN RM   SIZE RO TYPE MOUNTPOINT
sda      8:0    0 223.6G  0 disk
├─sda1   8:1    0 212.9G  0 part /
├─sda2   8:2    0     1K  0 part
└─sda5   8:5    0  10.8G  0 part [SWAP]
sdb      8:16   1  59.5G  0 disk
└─sdb1   8:17   1  59.5G  0 part /media/user/45828acd
```
At this example `sdb` was added after installing the SD card.

The location of SD card will be `/dev/sdb` in this example.
Typically it is similar to `/dev/sdb` or `/dev/mmcblk0`.

The `${dev-sd}` in following instruction is the location of SD card.
Replace `${dev-sd}` with `/dev/sdb` or `/dev/mmcblk0`, or what ever it is for your Linux machine.

Backup any data in SD card. The contents of SD will be completely erased.

Zap all partitions.
```sh
$ sudo ./sgdisk -Z ${dev-sd}
```
Make partition 1 - 32M 5202(SiFive bootloader).
```sh
$ sudo ./sgdisk --new=1:2048:67583   ${dev-sd}
$ sudo ./sgdisk --typecode=1:5202    ${dev-sd}
```
Make partition 4 - 128K 5201(SiFive FSBL).
```sh
$ sudo ./sgdisk --new=4:67584:67839 ${dev-sd}
$ sudo ./sgdisk --typecode=4:5201   ${dev-sd}
```
Make partition 2 - Linux 7.3G.
```sh
$ sudo ./sgdisk --new=2:264192:15500000 ${dev-sd}
$ sudo ./sgdisk --typecode=2:8300   ${dev-sd}
```
If the SD card is 16GB, change `--new=2:264192:15500000` above to `--new=2:264192:30370000`


Check the created partitions with the gptfdisk tool by:

```sh
$ sudo ./gdisk -l ${dev-sd}		#where ${dev-sd} is the location of the SD device
$ cd ..
```

It should show three partitions similar bellow.

```
	Number	Start (sector)	End (sector)	Size		Code	Name
	1	2048		67583		32.0 MiB	5202	SiFive bare-metal (...
	2	264192		15500000	7.4 GiB		8300	Linux filesystem
	4	67584		67839		128.0 KiB	5201	SiFive FSBL (first-...
```

### Write bootloader, FPGAfsbl.bin and bbl.bin, to SD card

Move to your keystone folder.
Write the bbl.bin to the 1st partition of the SD card, where the ${dev-sd-p1} is the 1st partition of the SD device.

The ${dev-sd-p1} will be `/dev/sdb1` or `/dev/mmcblk0p1` if the location of the SD card is `/dev/sdb` or `/dev/mmcblk0`.
Be careful not to use `/dev/sdb` or `/dev/mmcblk0` instead of `/dev/sdb1` or `/dev/mmcblk0p1`.
It will destroy the partitions created with above instructions in the SD card.

```sh
$ cd keystone
$ sudo dd if=hifive-work/bbl.bin of=${dev-sd-p1} bs=4096 conv=fsync
$ cd ..
```

Write the FPGAfsbl.bin to the 4th partition of the SD card, where the ${dev-sd-p4} is the 4th partition of the SD device.

The ${dev-sd-p4} will be `/dev/sdb4` or `/dev/mmcblk0p4` if the location of the SD card is `/dev/sdb` or `/dev/mmcblk0`.
Be careful not to use `/dev/sdb` or `/dev/mmcblk0` instead of `/dev/sdb4` or `/dev/mmcblk0p4`.

```sh
$ cd freedom/bootrom/freedom-u540-c000-bootloader/
$ sudo dd if=FPGAfsbl.bin of=${dev-sd-p4} bs=4096 conv=fsync
```

## Booting keystone on vc707

Insert the SD card on vc707 and powerup.

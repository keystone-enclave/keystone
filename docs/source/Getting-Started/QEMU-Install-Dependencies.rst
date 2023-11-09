Install Dependencies
----------------------------

We tested Keystone with QEMU Ubuntu 16.04/18.04/20.04 and derivatives.

Ubuntu
#######################

Install the following packages.

::

  sudo apt update
  sudo apt install autoconf automake autotools-dev bc \
  bison build-essential curl expat libexpat1-dev flex gawk gcc git \
  gperf libgmp-dev libmpc-dev libmpfr-dev libtool texinfo tmux \
  patchutils zlib1g-dev wget bzip2 patch vim-common lbzip2 python3 \
  pkg-config libglib2.0-dev libpixman-1-dev libssl-dev screen \
  device-tree-compiler expect makeself unzip cpio rsync cmake ninja-build p7zip-full

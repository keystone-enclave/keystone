Install Dependencies
----------------------------

We tested Keystone with QEMU Ubuntu 16.04/18.04 and derivatives.

Ubuntu
#######################

::

  sudo apt update
  sudo apt install autoconf automake autotools-dev bc \
  bison build-essential curl expat libexpat1-dev flex gawk gcc git \
  gperf libgmp-dev libmpc-dev libmpfr-dev libtool texinfo tmux \
  patchutils zlib1g-dev wget bzip2 patch vim-common lbzip2 python \
  pkg-config libglib2.0-dev libpixman-1-dev libssl-dev screen \
  device-tree-compiler expect makeself unzip cpio rsync cmake p7zip-full

.. note::
  You need Git version >= 2.11.0 to use ``./fast-setup.sh``, because the script uses
  ``--shallow-clone`` for faster submodule initializtion.

In order to use the Rust version of the security monitor (only available for 0.X versions), you will also need to install `rustup <https://rustup.rs>`_ and `llvm-9 <https://apt.llvm.org/>`_ (available in Ubuntu repositories in versions 18.04 and above). Then, run the following commands:

::

  rustup toolchain install nightly
  rustup +nightly component add rust-src
  rustup +nightly target add riscv64gc-unknown-none-elf
  cargo +nightly install cargo-xbuild


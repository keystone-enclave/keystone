FROM ubuntu:18.04
ARG CHECKOUT=master
RUN apt update

RUN DEBIAN_FRONTEND="noninteractive" apt-get -y install tzdata

RUN apt -y install autoconf automake autotools-dev bc \
    bison build-essential curl expat libexpat1-dev flex gawk gcc git \
    gperf libgmp-dev libmpc-dev libmpfr-dev libtool texinfo tmux \
    patchutils zlib1g-dev wget bzip2 patch vim-common lbzip2 python \
    pkg-config libglib2.0-dev libpixman-1-dev libssl-dev screen \
    device-tree-compiler expect makeself unzip cpio rsync cmake

RUN git clone https://github.com/keystone-enclave/keystone /keystone
RUN cd /keystone && \
    git checkout $CHECKOUT && \
    rmdir linux qemu buildroot && \
    ./fast-setup.sh && \
    . ./source.sh && \
    rm -rf firesim-riscv-tools-prebuilt-* && \
    rm -f 2.0.tar.gz && \
    mkdir -p build && \
    cd build && \
    cmake .. && \
    make -j$(nproc)

ENTRYPOINT cd /keystone && . ./source.sh && cd /keystone/build && make run-tests

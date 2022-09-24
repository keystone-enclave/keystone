#!/usr/bin/env bash
DIR="/usr/src/gtest"
if [ ! -d "$DIR" ]; then
	apt-get -y install libgtest-dev
fi
prevdir=$(pwd)
GLIB=$DIR/libgtest.so
if [ ! -d "$GLIB" ]; then
	cd $DIR
	cmake CMakeLists.txt -DBUILD_SHARED_LIBS=ON
	make
	cp *.so /usr/lib
	cd $prevdir
fi

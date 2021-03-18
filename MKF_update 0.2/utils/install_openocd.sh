#!/bin/bash

sudo apt-get remove openocd

unset CXX

mkdir -p ~/sandbox

cd ~/sandbox

mkdir openocd

git clone git://repo.or.cz/openocd.git

cd openocd

sudo apt-get install make libtool pkg-config autoconf automake texinfo libusb-1.0-0-dev

./bootstrap

./configure

make

sudo make install


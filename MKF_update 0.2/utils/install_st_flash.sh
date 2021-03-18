#!/bin/bash

sudo apt-get install libusb-1.0-0-dev git cmake

cd ~
git clone https://github.com/texane/stlink stlink.git
cd stlink.git
make
sudo cp ./build/Release/st-flash /usr/bin
sudo cp ./build/Release/st-info /usr/bin
sudo cp ./build/Release/src/gdbserver/st-util /usr/bin
sudo cp ./etc/udev/rules.d/*.rules /etc/udev/rules.d
sudo restart udev

#!/bin/bash

# setup steps taken from https://github.com/zeromq/goczmq

sudo apt-get install build-essential gcc make pkg-config -y

wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.3.tar.gz
wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.3.tar.gz.sig
wget https://download.libsodium.org/jedi.gpg.asc
gpg --import jedi.gpg.asc
gpg --verify libsodium-1.0.3.tar.gz.sig libsodium-1.0.3.tar.gz
tar zxvf libsodium-1.0.3.tar.gz
pushd libsodium-1.0.3
./configure; make check
sudo make install
sudo ldconfig
popd

wget http://download.zeromq.org/zeromq-4.1.1.tar.gz
tar zxvf zeromq-4.1.1.tar.gz
pushd zeromq-4.1.1
./configure --with-libsodium; make; make check
sudo make install
sudo ldconfig
popd

wget http://download.zeromq.org/czmq-3.0.1.tar.gz
tar zxvf czmq-3.0.1.tar.gz
pushd czmq-3.0.1
./configure; make check
sudo make install
sudo ldconfig
popd

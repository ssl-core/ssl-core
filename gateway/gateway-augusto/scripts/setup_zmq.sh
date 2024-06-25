apt-get update
apt-get install build-essential gcc make pkg-config -y

cd /tmp
wget https://download.libsodium.org/libsodium/releases/libsodium-1.0.19-stable.tar.gz
tar zxvf libsodium-1.0.19-stable.tar.gz
pushd libsodium-1.0.19-stable
./configure
make && make check
make install
popd
rm -rf libsodium-*

apt-get install libczmq-dev -y
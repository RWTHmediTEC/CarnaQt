#!/bin/bash
set -e

mkdir -p build/make_debug
mkdir -p build/make_release

cd build/make_debug
cmake -DCMAKE_BUILD_TYPE=Debug -DBUILD_DOC=ON -DBUILD_TEST=ON -DBUILD_DEMO=ON ../..
make

cd ../make_release
cmake -DCMAKE_BUILD_TYPE=Release -DBUILD_DOC=OFF -DBUILD_TEST=ON -DBUILD_DEMO=ON ../..
make

echo
echo "** Ready to install."

sudo make install
cd ../make_debug
sudo make install

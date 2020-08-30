#!/bin/bash
rm -f ./bin/sim

mkdir build
cd build
cmake ..
make -j5

cd ..
mkdir bin
mv ./build/src/sim ./bin

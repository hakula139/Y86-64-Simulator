#!/bin/bash

rm -f ./bin/sim
mkdir -p build && cd build && cmake .. && make -j5 && cd ..
mkdir -p bin && mv ./build/src/sim ./bin


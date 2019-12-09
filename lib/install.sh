rm -rf build
mkdir build
cd build
cmake ..
make
cd ..
mv ./build/src/sim .

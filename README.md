# Y86-64 Simulator

A Y86-64 processor simulator in C++.

## Usage

### Windows

```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
mingw32-make
src/sim.exe ../test/objects/<filename>.yo
```

## Requirements

- CMake 3.2 or above
- GNU make 4.0 or above
- gcc 4.8 or above (MinGW-w64 on Windows)

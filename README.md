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

- CMake
- GNU make
- gcc (MinGW-w64 on Windows)

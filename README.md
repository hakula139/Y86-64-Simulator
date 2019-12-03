# Y86-64 Simulator

A Y86-64 processor simulator written in C++11.

## Getting Started

### 0. Prerequisites

- [CMake](https://cmake.org/download) 3.2 or above
- [GNU make](https://www.gnu.org/software/make) 4.0 or above
- [GCC](https://gcc.gnu.org/releases.html) 4.8 or above

For Windows, try [MinGW-w64](https://sourceforge.net/projects/mingw-w64).

### 1. Installation

#### 1.1 Windows

```bash
./install.ps1
```

#### 1.2 Linux

```bash
./install.sh
```

You may need to grant yourself the execute permission using the command `chmod`.

```bash
chmod +x install.sh
```

### 2. Usage

You may check the manual simply by passing no argument.

```bash
./sim
```

#### 2.1 Windows

```bash
./sim.exe "./test/objects/prog1.yo"
```

#### 2.2 Linux

```bash
./sim "./test/objects/prog1.yo"
```

### 3. Clean

#### 3.1 Windows

```bash
./clean.ps1
```

#### 3.2 Linux

```bash
./clean.sh
```

## Contributors

- **Hakula Chen** - Fudan University
- **Jiaqi Dai** - Fudan University

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](https://github.com/hakula139/Y86-64-Simulator/blob/master/LICENSE) file for details.

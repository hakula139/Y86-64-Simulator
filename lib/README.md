# Y86-64 Simulator

A Y86-64 processor simulator written in C++11.

## Getting Started

### 0. Prerequisites

- [CMake](https://cmake.org/download) 2.8 or above
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

```bash
./sim input_file [output_dir]
```

#### Arguments

##### input_file

`input_file` is the path to your input file. Currently only .yo files are accepted.

##### output_dir

*Optional.* `output_dir` is the directory for all outputs. Please make sure it exists. By default it will be set to the root folder of the executable `sim`.

#### Examples

```bash
./sim "../test/objects/prog1.yo"
```

```bash
./sim "../test/objects/asum.yo" "./temp"
```

#### Manual

Check the manual simply by passing no argument.

```bash
./sim
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

- [**Hakula Chen**](https://github.com/hakula139)<[i@hakula.xyz](mailto:i@hakula.xyz)> - Fudan University
- [**Jiaqi Dai**](https://github.com/jqdai) - Fudan University

## License

This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](https://github.com/hakula139/Y86-64-Simulator/blob/master/LICENSE) file for details.
